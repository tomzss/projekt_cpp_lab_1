#pragma once

#include <deque>
#include <SFML/Graphics/Sprite.hpp>
#include "sfml/Rect.hpp"
#include "Image.hpp"
#include "SizedTexture.hpp"

using Images = std::deque<Image>;

/// Images displayed on screen as gallery (as "tiles")
class ImageGallery {
public:
    ImageGallery(Images const &images, Rect<unsigned> const &screenArea, unsigned columns, float gapSize);

    /// Images to display
    ND Images const &getImages() const;

    /// Area on screen do display images on (in pixels)
    ND Rect<unsigned> const &getScreenArea() const;

    /// Number of columns in grid of images
    ND unsigned int getColumns() const;

    /// Distance between images (in pixels)
    ND float getGapSize() const;

    /// Width of single image in gallery
    ND float imageSize() const;

    /// Move view by scrollDiff pixels down
    void scroll(float scrollDiff);

    void setArea(Rect<unsigned> const &newArea);

    /// Maps global position to local. If position is outside area, returns std::nullopt
    ND std::optional<Vector2u> globalPosToInAreaPos(Vector2u const &) const;

    /// mousePos relative to area
    void pressed(Vector2u const &mousePos);

    void draw(sf::RenderTarget &target) const;

    ND Image const *getSelectedImage() const;
    void setColumnsCount(unsigned columns);

private:
    Rect<unsigned> screenArea;
    unsigned columns;
    float gapSize;
    Images const &images;
    Image const *selectedImage;
    float scrollPoint;

    void drawBorder(sf::RenderTarget &, Rect<float> const &area) const;
    ND Image const *getImageByGridCoords(std::size_t x, std::size_t y) const;

    /// Prevents scrolling out of view. Call after changing scroll/view
    void fixScrollPoint();
    unsigned long rows() const;
};
