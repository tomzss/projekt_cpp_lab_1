#include "ImageDisplay.hpp"

Images const &ImageGallery::getImages() const {
    return images;
}

sf::FloatRect const &ImageGallery::getScreenArea() const {
    return screenArea;
}

unsigned ImageGallery::getColumns() const {
    return columns;
}

float ImageGallery::getMargin() const {
    return margin;
}

ImageDisplay::ImageDisplay(ImageGallery const &gallery_) :
        gallery{gallery_},
        iter{gallery_.getImages().begin()},
        current_position{} {
}
