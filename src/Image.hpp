#pragma once

#include <functional>
#include <optional>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include "Nodiscard.hpp"
#include "filesystem.hpp"
#include "Tag.hpp"

class Image {
public:
    using Tags = std::vector<std::reference_wrapper<Tag const>>;

    /// Load image from file
    static std::optional<Image> tryLoad(fsys::path path);

    /// Access image data
    ND Tags const &getTags();
    ND fsys::path const &getPath() const;
    ND sf::Texture const &getTexture() const;

    /// Add tag to image. May add
    void addTag(Tag const &tag);

    static auto constexpr supportedFormats = {".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic"};

private:
    Image(fsys::path path, sf::Texture const &sfTexture, Tags tags);

    fsys::path path;
    sf::Texture sfTexture;
    Tags tags;
};
