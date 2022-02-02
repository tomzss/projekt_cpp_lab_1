#include "Directory.hpp"

#include <utility>

bool containsImages(fsys::directory_entry const &directory) {
    for (auto &file: fsys::directory_iterator{directory}) {
        if (file.path().filename().string().starts_with('.'))
            return false;
        if (file.is_directory() and not containsImages(file))
            return false;
        if (Image::isFormatSupported(file.path().extension().string()))
            return true;
    }
    return false;
}

Directory::Directory(fsys::path path_) :
        myPath{std::move(path_)} {
    loadImages();

    if (myPath.has_parent_path())
        availableDirectories.emplace_back(myPath.parent_path());

    for (auto &file: fsys::directory_iterator{myPath})
        if (file.is_directory() and containsImages(file))
            availableDirectories.emplace_back(file);
}

std::deque<Image> const &Directory::getImages() const {
    return images;
}

void Directory::loadImages() {
    for (auto const &iter: fsys::directory_iterator{myPath})
        images.emplace_back(Image::loadFromFile(iter));
}

std::deque<fsys::path> const &Directory::getAvailableDirectories() const {
    return availableDirectories;
}
