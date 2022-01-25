#include "Image.hpp"

class TextureLoader{
public:
    std::optional<std::reference_wrapper<sf::Texture const>> getTexture() const {
        if (getState() == State::ready)
            return std::ref(sfTexture);
        return std::nullopt;
    }

private:
    std::map<fsys::path, sf::Texture> textures;
    std::thread loadingThread;
};

Image Image::loadFromFile(fsys::path path) {
    auto sfImage = sf::Image{};
    if (not isFormatSupported(path.extension().string()))
        return std::nullopt;
    if (not sfImage.loadFromFile(path.string()))
        return std::nullopt;
    if (sfImage.getSize().x > sf::Texture::getMaximumSize() or sfImage.getSize().y > sf::Texture::getMaximumSize())
        return std::nullopt;

    auto texture = sf::Texture{};
    texture.loadFromImage(sfImage);
    return Image{std::move(path), texture};
}

bool Image::isFormatSupported(std::string_view format) {
    return std::ranges::find(supportedFormats, format) != supportedFormats.end();
}

fsys::path const &Image::getPath() const {
    return path;
}

std::optional<std::reference_wrapper<sf::Texture const>> Image::getTexture() const {
    if (getState() == State::ready)
        return std::ref(sfTexture);
    return std::nullopt;
}

Image::Image(fsys::path path, sf::Texture const &sfTexture_) :
        path{std::move(path)},
        sfTexture{sfTexture_},
        stillLoading{true} {
    sfTexture.setSmooth(true);
}

Image::State Image::getState() const {
    if (not loaded)
        return State::loading;

    auto const &size = sfTexture.getSize();
    if (size.x == 0 or size.y == 0)
        return State::invalid;

    return State::ready;
}
