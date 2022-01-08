#include "ImageDisplay.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <cmath>

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
    auto const &columnsF = static_cast<float>(columns);
    auto const &gapsCount = columnsF - 1;
    auto const &gapsSummarySize = gapsCount * gapSize;
    auto const &imagesSummaryWidth = getScreenArea().width - gapsSummarySize;
    return imagesSummaryWidth / columnsF;
}

void ImageGallery::draw(sf::RenderTarget &target, float scrollPoint) {
    auto const firstImageIndexCandidate = std::ptrdiff_t(std::floor(scrollPoint / getImageWidth()));
    auto const lastImageIndexCandidate = std::ptrdiff_t(
            std::ceil((scrollPoint + screenArea.height) / getImageWidth()));

    auto const firstImageIndex = std::max<std::ptrdiff_t>(
            firstImageIndexCandidate, static_cast<std::ptrdiff_t>(0));

    auto const lastImageIndex = std::min(
            lastImageIndexCandidate, static_cast<std::ptrdiff_t>((images.size() - 1)));

    auto const &startPosition = sf::Vector2f{screenArea.top, screenArea.left};

    auto column = 0u;
    auto row = 0u;
    auto firstImageIter = images.begin() + firstImageIndex;
    auto lastImageIter = images.begin() + lastImageIndex;
    for (auto imageIter = firstImageIter; imageIter != lastImageIter; ++imageIter) {
        auto const &image = *imageIter;

        if (column >= columns) {
            column = 0;
            row += 1;
        }

        auto const &texture = SizedTexture::byOuterBox(image.getTexture(), {getImageWidth(), getImageWidth()});
        auto const &positionOffset = sf::Vector2f{static_cast<float>(column) * (getImageWidth() + gapSize),
                                                  static_cast<float>(row) * (getImageWidth() + gapSize) +
                                                  std::fmod(static_cast<float>(firstImageIndex), getImageWidth())};
        auto const &position = startPosition + positionOffset;

        auto sprite = sf::Sprite{};
        sprite.setPosition(position);
        texture.applyScaleAndTexture(sprite);

        auto const &imageHeight = texture.getSize().y;
        if (position.y + imageHeight > screenArea.top + screenArea.height)
            break; // we're out of visible area

        target.draw(sprite);

        column += 1;
    }
}

ImageGallery::ImageGallery(Images &images, sf::FloatRect const &screenArea, unsigned columns, float gapSize) :
        screenArea(screenArea),
        columns(columns),
        gapSize(gapSize),
        images(images) {
}
