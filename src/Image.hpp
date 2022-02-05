#pragma once

#include <functional>
#include <optional>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include "Nodiscard.hpp"
#include "filesystem.hpp"

class Image {
public:
    /// Load image from file
    static std::optional<Image> tryLoad(fsys::path path);

    /// Access image data
    ND fsys::path const &getPath() const;
    ND sf::Texture const &getTexture() const;

    /// Check if format (eg. ".png") is supported.
    static bool isFormatSupported(sf::String const&);

private:
    /// Constructor is private. Use tryLoad to create Image.
    Image(fsys::path path, sf::Texture const &sfTexture);

    fsys::path path;
    sf::Texture sfTexture;

    /// From SFML docs.
    static auto constexpr supportedFormats = {".bmp", ".png", ".tga", ".jpg", ".psd", ".hdr", ".pic"};
};
