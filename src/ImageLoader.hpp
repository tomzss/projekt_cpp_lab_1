#pragma once

#include <optional>
#include "filesystem.hpp"
#include "Image.hpp"

class ImageLoader {
public:
    /// Creates object to load images from certain directory
    explicit ImageLoader(fsys::directory_iterator iter);

    /// Loads next image and returns it.
    /// Returns nullopt when all images in directory were already loaded.
    std::optional<Image> nextImage();

private:
    fsys::directory_iterator iter;
};
