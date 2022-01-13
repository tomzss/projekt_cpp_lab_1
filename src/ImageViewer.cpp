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
    auto const position = (screenArea.position() + screenArea.size() / 2u).cast<float>();
    auto const texture = SizedTexture::byOuterBox(sfTexture, screenArea.size().cast<float>());
    texture.applyScaleAndTextureAndCenter(sprite, position);
    target.draw(sprite);
}

void ImageViewer::setScreenArea(Rect<unsigned> const &screenArea_) {
    screenArea = screenArea_;
}
