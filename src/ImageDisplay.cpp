#include "ImageDisplay.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <cmath>
#include <GL/gl.h>

Images const &ImageGallery::getImages() const {
    return images;
}

sf::FloatRect const &ImageGallery::getScreenArea() const {
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

void ImageGallery::draw(sf::RenderTarget &target, float scrollPoint) const {

    auto const imageHeight = getImageWidth(); // images are square
    auto const rowsCount = images.size() / columns;
    auto const firstVisibleRow = static_cast<std::size_t>(std::max<float>(
            0, std::floor(scrollPoint / imageHeight)));
    auto const lastVisibleRow = std::min<std::size_t>(
            static_cast<int>(images.size() - 1), std::ceil((scrollPoint + screenArea.height) / imageHeight));

    auto const &startPosition = sf::Vector2f{screenArea.left, screenArea.top};

    auto getImageByGridCoords = [&](auto x, auto y) -> auto & {
        return images.at(y * columns + x);
    };

    glEnable(GL_SCISSOR_TEST);
    glScissor(
            static_cast<int>(screenArea.left),
            target.getSize().y - (screenArea.top + screenArea.height),
            static_cast<int>(screenArea.width),
            static_cast<int>(screenArea.height));
    for (auto row = firstVisibleRow; row != lastVisibleRow; ++row) {
        auto const rowPosY = static_cast<float>(row) * (imageHeight + gapSize) - scrollPoint;
        for (auto column = 0u; column != columns; ++column) {
            if (row * columns + columns > images.size()) {
                glDisable(GL_SCISSOR_TEST);

                return;
            }

            auto const &image = getImageByGridCoords(column, row);

            auto const &texture = SizedTexture::byOuterBox(image.getTexture(), {getImageWidth(), getImageWidth()});
            auto const &positionOffset = sf::Vector2f{static_cast<float>(column) * (getImageWidth() + gapSize),
                                                      rowPosY};
            auto const &position = startPosition + positionOffset;

            auto sprite = sf::Sprite{};
            sprite.setPosition(position);
            texture.applyScaleAndTexture(sprite);

            auto const &imageHeight = texture.getSize().y;

            target.draw(sprite);
        }
    }
    glDisable(GL_SCISSOR_TEST);
}

ImageGallery::ImageGallery(Images &images, sf::FloatRect const &screenArea, unsigned columns, float gapSize) :
        screenArea(screenArea),
        columns(columns),
        gapSize(gapSize),
        images(images) {
}
