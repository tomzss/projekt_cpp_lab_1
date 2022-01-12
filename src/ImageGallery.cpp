#include "ImageGallery.hpp"
#include "Subtarget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <cmath>
#include <SFML/Graphics/RectangleShape.hpp>

Images const &ImageGallery::getImages() const {
    return images;
}

Rect<unsigned> const &ImageGallery::getScreenArea() const {
    return screenArea;
}

unsigned ImageGallery::getColumns() const {
    return columns;
}

float ImageGallery::getGapSize() const {
    return gapSize;
}

float ImageGallery::getImageWidth() const {
    auto const columnsF = static_cast<float>(columns);
    auto const gapsCount = columnsF - 1;
    auto const gapsSummarySize = gapsCount * gapSize;
    auto const imagesSummaryWidth = static_cast<float>(getScreenArea().width()) - gapsSummarySize;
    return imagesSummaryWidth / columnsF;
}

void ImageGallery::draw(sf::RenderTarget &target) const {
    auto subtarget = Subtarget{target, screenArea};

    auto const imageSize = getImageWidth(); // images are square
    auto const rowWidth = getImageWidth() + gapSize; // images are square
    auto const firstVisibleRow = static_cast<std::size_t>(std::max<float>(
            0, std::floor(scrollPoint / imageSize)));
    auto const lastVisibleRow = std::min<std::size_t>(
            static_cast<int>(images.size() - 1),
            std::ceil((scrollPoint + static_cast<float>(screenArea.height())) / imageSize));

    auto getImageByGridCoords = [&](auto x, auto y) -> auto & {
        return images.at(y * columns + x);
    };

    for (auto row = firstVisibleRow; row != lastVisibleRow; ++row) {
        auto const rowPosY = static_cast<float>(row) * rowWidth - scrollPoint;
        for (auto column = 0u; column != columns; ++column) {
            if (row * columns + columns > images.size())
                return;

            auto const &image = getImageByGridCoords(column, row);
            auto const &texture = SizedTexture::byOuterBox(image.getTexture(), {imageSize, imageSize});
            auto const &position = Vector2f{static_cast<float>(column) * rowWidth, rowPosY};

            auto sprite = sf::Sprite{};
            sprite.setPosition(position);
            texture.applyScaleAndTexture(sprite);

            subtarget.draw(sprite);
            if (&image == selectedImage)
                drawBorder(target, {position, texture.getSize()});
        }
    }
}

void ImageGallery::drawBorder(sf::RenderTarget &target, Rect<float> const &area) const {
    auto shape = sf::RectangleShape{};
    auto const halfGapVec = sf::Vector2f{gapSize, gapSize} / 2.f;
    shape.setSize(area.size() + halfGapVec);
    shape.setPosition(area.position() - halfGapVec);
    shape.setOutlineThickness(gapSize);
    shape.setOutlineColor(sf::Color{255, 255, 255, 50});
    target.draw(shape);
}

ImageGallery::ImageGallery(Images &images, Rect<unsigned> const &screenArea, unsigned columns, float gapSize) :
        screenArea{screenArea},
        columns{columns},
        gapSize{gapSize},
        images{images},
        selectedImage{nullptr},
        scrollPoint{0} {
}

void ImageGallery::scroll(float scroll) {
    scrollPoint -= scroll;
}

void ImageGallery::setArea(Rect<unsigned> const &newArea) {
    screenArea = newArea;
}

Image *ImageGallery::getSelectedImage() const {
    return selectedImage;
}
