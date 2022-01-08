#pragma once

#include <deque>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Image.hpp"
#include "SizedTexture.hpp"

using Images = std::deque<Image>;
using ImageIter = Images::const_iterator;

/// Images displayed on screen as gallery (as "tiles")
class ImageGallery {
public:
    ImageGallery(Images &images, sf::FloatRect const &screenArea, unsigned columns, float gapSize);

    /// Images to display
    ND Images const &getImages() const;

    /// Area on screen do display images on (in pixels)
    ND sf::FloatRect const &getScreenArea() const;

    /// Number of columns in grid of images
    ND unsigned int getColumns() const;

    /// Distance between images (in pixels)
    ND float getGapSize() const;

    /// Width of single image in gallery
    ND float getImageWidth() const;

    void draw(sf::RenderTarget &target, float startPoint);

private:
    sf::FloatRect screenArea;
    unsigned columns;
    float gapSize;
    Images& images;
};
