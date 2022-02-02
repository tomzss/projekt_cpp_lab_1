#pragma once

#include <functional>
#include <optional>
#include <vector>
#include <thread>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include "Nodiscard.hpp"
#include "filesystem.hpp"

class ImageData;

class Image {
public:
    enum class State {
        loading, ready, invalid
    };

    /// Check if file format (eg. ".png") is supported.
    static bool isFormatSupported(std::string_view);

    /// Start loading Image.
    /// Image will be in the `loading` state for some time,
    /// then state will change to either `ready` or `invalid`.
    static Image loadFromFile(fsys::path path);

    ND State getState() const;

    /// Returns texture only in `ready` state.
    ND std::optional<std::reference_wrapper<sf::Texture const>> getTexture() const;

    ND fsys::path getPath() const;

private:
    explicit Image(std::shared_ptr<ImageData const> data);

    std::shared_ptr<ImageData const> data;

    static auto constexpr supportedFormats = {".bmp", ".png", ".tga", ".jpg", ".psd", ".hdr", ".pic"};
};
