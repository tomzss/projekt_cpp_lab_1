#include "Image.hpp"

Image::Tags const &Image::getTags() {
    return tags;
}

std::optional<Image> Image::tryLoad(fsys::path path) {
    auto sfImage = sf::Image{};
    if (not isFormatSupported(path.extension().string()))
        return std::nullopt;
    if (not sfImage.loadFromFile(path.string()))
        return std::nullopt;
    if (sfImage.getSize().x > sf::Texture::getMaximumSize() or sfImage.getSize().y > sf::Texture::getMaximumSize())
        return std::nullopt;

    auto texture = sf::Texture{};
    texture.loadFromImage(sfImage);
    return Image{std::move(path), texture, {}};
}

bool Image::isFormatSupported(sf::String const &format) {
    return std::ranges::find(supportedFormats, format) != supportedFormats.end();
}

void Image::addTag(Tag const &tag) {
    tags.emplace_back(tag);
}

fsys::path const &Image::getPath() const {
    return path;
}

sf::Texture const &Image::getTexture() const {
    return sfTexture;
}

Image::Image(fsys::path path, sf::Texture const &sfTexture_, Image::Tags tags) :
        path{std::move(path)},
        sfTexture{sfTexture_},
        tags{std::move(tags)} {
    sfTexture.setSmooth(true);
}
