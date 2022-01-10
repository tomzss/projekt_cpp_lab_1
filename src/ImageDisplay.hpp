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
    ImageGallery(Images &images, sf::Rect<unsigned> const &screenArea, unsigned columns, float gapSize);

    /// Images to display
    ND Images const &getImages() const;

    /// Area on screen do display images on (in pixels)
    ND sf::Rect<unsigned> const & getScreenArea() const;

    /// Number of columns in grid of images
    ND unsigned int getColumns() const;

    /// Distance between images (in pixels)
    ND float getGapSize() const;

    /// Width of single image in gallery
    ND float getImageWidth() const;

    /// Move view by scrollDiff pixels down
    void scroll(float scrollDiff);

    void setArea(sf::Rect<unsigned> const& newArea);

    void draw(sf::RenderTarget &target) const;

private:
    sf::Rect<unsigned> screenArea;
    unsigned columns;
    float gapSize;
    Images& images;
    float scrollPoint;
};
