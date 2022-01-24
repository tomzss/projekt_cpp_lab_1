#include "Directory.hpp"

#include <utility>

Directory::Directory(fsys::path path_) :
        myPath{std::move(path_)} {
    loadImages();
    for (auto &dir: fsys::directory_iterator{myPath})
        if (dir.is_directory() and not dir.path().string().starts_with('.'))
            availableDirectories.emplace_back(dir);
}

std::deque<Image> const &Directory::getImages() const {
    return images;
}

void Directory::loadImages() {
    for (auto const &iter: fsys::directory_iterator{myPath})
        if (auto const &image = Image::tryLoad(iter))
            images.emplace_back(*image);
}

std::deque<fsys::path> const &Directory::getAvailableDirectories() const {
    return availableDirectories;
}
