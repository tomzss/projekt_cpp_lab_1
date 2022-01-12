#include "ImageViewer.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "SizedTexture.hpp"

ImageViewer::ImageViewer(sf::Texture const &texture, Rect<unsigned> const &area) :
        sfTexture{texture},
        screenArea{area} {
}

void ImageViewer::draw(sf::RenderTarget &target) {
    auto sprite = sf::Sprite{};
    sprite.setPosition(screenArea.position().cast<float>());
    auto const texture = SizedTexture::byOuterBox(sfTexture, screenArea.size().cast<float>());
    target.draw(sprite);
}
