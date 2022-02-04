#include "Directory.hpp"

/// File to ignore
bool outOfInterest(fsys::directory_entry const &file) {
    // no access
    if ((file.status().permissions() & fsys::perms::others_read) == fsys::perms::none)
        return true;

    // link (to avoid loops)
    if (file.is_symlink())
        return true;

    // hidden file
    if (file.path().filename().string().starts_with('.'))
        return true;

    return false;
}

/// Is an displayable image or a directory containing images
bool isImageRelated(fsys::directory_entry const &file) {
    if (outOfInterest(file))
        return false;

    if (Image::isFormatSupported(file.path().extension().string()))
        return true;

    if (file.is_directory())
        for (auto &subFile: fsys::directory_iterator{file})
            if (isImageRelated(subFile))
                return true;

    return false;
}

Directory::Directory(fsys::path path_) :
        myPath{std::move(path_)} {
    loadImages();

    if (myPath.has_parent_path())
        availableDirectories.emplace_back(myPath.parent_path());

    for (auto &file: fsys::directory_iterator{myPath})
        if (isImageRelated(file) and file.is_directory())
            availableDirectories.emplace_back(file);
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
