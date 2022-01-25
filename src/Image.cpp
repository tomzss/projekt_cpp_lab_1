#include "Image.hpp"

#include <map>
#include <list>
#include <deque>

struct ImageData {
    fsys::path path;
    sf::Texture texture;
    Image::State state = Image::State::loading;
};

std::optional<sf::Texture> tryLoad(fsys::path path) {
    auto sfImage = sf::Image{};
    if (not Image::isFormatSupported(path.extension().string()))
        return std::nullopt;
    if (not sfImage.loadFromFile(path.string()))
        return std::nullopt;
    if (sfImage.getSize().x > sf::Texture::getMaximumSize() or sfImage.getSize().y > sf::Texture::getMaximumSize())
        return std::nullopt;

    auto texture = sf::Texture{};
    texture.loadFromImage(sfImage);
    return texture;
}

class TextureLoader {
public:
    ND std::shared_ptr<ImageData const> getTextureData(fsys::path path) {
        // is it already known?
        auto iter = std::ranges::find_if(textures, [&](auto &i) { return i->path == path; });

        // yeah, we tried to load it
        if (iter != textures.end())
            return *iter;

        // it's new
        return textures.emplace_front(std::make_shared<ImageData>(path, sf::Texture{}, Image::State::loading));
    }

    void loading_function() {
        for (auto iter = texturesToLoad.begin(); iter != texturesToLoad.end(); ++iter) {
            auto &ptr = *iter;
            if (auto dataPtr = ptr.lock()) {
                auto &texture = *dataPtr;
                if (auto optTexture = tryLoad(path)) {
                    texture = *optTexture;
                }
            } else {
            }
        }
    }


private:
    std::list<std::shared_ptr<ImageData>> textures;
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
