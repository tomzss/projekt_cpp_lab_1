#include "ImageDisplay.hpp"
#include "Subtarget.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <cmath>

Images const &ImageGallery::getImages() const {
    return images;
}

sf::Rect<unsigned> const &ImageGallery::getScreenArea() const {
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
    auto const imagesSummaryWidth = getScreenArea().width - gapsSummarySize;
    return imagesSummaryWidth / columnsF;
}

void ImageGallery::draw(sf::RenderTarget &target) const {
    auto subtarget = Subtarget{target, screenArea};

    auto const imageHeight = getImageWidth(); // images are square
    auto const rowsCount = images.size() / columns;
    auto const firstVisibleRow = static_cast<std::size_t>(std::max<float>(
            0, std::floor(scrollPoint / imageHeight)));
    auto const lastVisibleRow = std::min<std::size_t>(
            static_cast<int>(images.size() - 1), std::ceil((scrollPoint + screenArea.height) / imageHeight));

    auto getImageByGridCoords = [&](auto x, auto y) -> auto & {
        return images.at(y * columns + x);
    };

    for (auto row = firstVisibleRow; row != lastVisibleRow; ++row) {
        auto const rowPosY = static_cast<float>(row) * (imageHeight + gapSize) - scrollPoint;
        for (auto column = 0u; column != columns; ++column) {
            if (row * columns + columns > images.size())
                return;

            auto const &image = getImageByGridCoords(column, row);

            auto const &texture = SizedTexture::byOuterBox(image.getTexture(), {getImageWidth(), getImageWidth()});
            auto const &position = sf::Vector2f{static_cast<float>(column) * (getImageWidth() + gapSize), rowPosY};

            auto sprite = sf::Sprite{};
            sprite.setPosition(position);
            texture.applyScaleAndTexture(sprite);

            subtarget.draw(sprite);
        }
    }
}

ImageGallery::ImageGallery(Images &images, sf::Rect<unsigned> const &screenArea, unsigned columns, float gapSize) :
        screenArea(screenArea),
        columns(columns),
        gapSize(gapSize),
        images(images),
        scrollPoint(0) {
}

void ImageGallery::scroll(float scroll) {
    scrollPoint -= scroll;
}

void ImageGallery::setArea(sf::Rect<unsigned> const &newArea) {
    screenArea = newArea;
}
