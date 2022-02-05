#pragma once

#include <deque>
#include "filesystem.hpp"
#include "Image.hpp"

/// Directory of images other directories (also containing images)
class Directory {
public:
    explicit Directory(fsys::path);

    // Images existing in this directory.
    ND std::deque<Image> const &getImages() const;
    ND std::deque<fsys::path> const &getAvailableDirectories() const;
    ND fsys::path const &getMyPath() const;

private:
    std::deque<Image> images;
    std::deque<fsys::path> availableDirectories;
    fsys::path myPath;

    void loadImages();
};
