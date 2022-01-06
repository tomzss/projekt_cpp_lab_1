#include "Image.hpp"

Image::Tags const &Image::getTags() {
    return tags;
}

std::optional<Image> Image::tryLoad(fsys::path path) {
    auto texture = sf::Texture{};
    if (not texture.loadFromFile(path.string()))
        return std::nullopt;
    return Image{std::move(path), texture, {}};
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

Image::Image(fsys::path path, sf::Texture const &sfTexture, Image::Tags tags) :
        path{std::move(path)},
        sfTexture{sfTexture},
        tags{std::move(tags)} {
}
