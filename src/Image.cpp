#include "Image.hpp"

Image::Tags const &Image::getTags() {
    return tags;
}

std::optional<Image> Image::tryLoad(fsys::path path) {
    auto texture = sf::Texture{};
    if (std::ranges::find(supportedFormats, path.extension()) == supportedFormats.end())
        return std::nullopt;
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
//    if(sfTexture.getSize().x==0)
//        loadI
    return sfTexture;
}

Image::Image(fsys::path path, sf::Texture const &sfTexture_, Image::Tags tags) :
        path{std::move(path)},
        sfTexture{sfTexture_},
        tags{std::move(tags)} {
    sfTexture.setSmooth(true);
}
