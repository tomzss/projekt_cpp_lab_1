#include "ImageLoader.hpp"

std::optional<Image> ImageLoader::nextImage() {
    if (iter == fsys::end(iter))
        return std::nullopt;
    auto oldIter = iter;
    ++iter;
    if (auto image = Image::tryLoad(*oldIter))
        return image;
    return nextImage();
}

ImageLoader::ImageLoader(fsys::directory_iterator iter) :
        iter(std::move(iter)) {
}
