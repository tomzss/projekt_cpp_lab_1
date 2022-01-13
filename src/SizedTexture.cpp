#include "SizedTexture.hpp"

bool isAreaWiderThanTexture(sf::Vector2f const &area, sf::Texture const &texture) {
    auto const &areaRatio = area.x / area.y;
    auto const &textureRatio = static_cast<float>(texture.getSize().x) / static_cast<float>(texture.getSize().y);
    return areaRatio > textureRatio;
}

SizedTexture SizedTexture::byWidth(sf::Texture const &texture, float width) {
    auto const &scale = width / static_cast<float>(texture.getSize().x);
    return SizedTexture{texture, scale};
}

SizedTexture SizedTexture::byHeight(sf::Texture const &texture, float height) {
    auto const &scale = height / static_cast<float>(texture.getSize().y);
    return SizedTexture{texture, scale};
}

SizedTexture SizedTexture::byOuterBox(sf::Texture const &texture, Vector2f const &outerBox) {
    if (isAreaWiderThanTexture(outerBox, texture))
        return byHeight(texture, outerBox.y);
    return byWidth(texture, outerBox.x);
}

SizedTexture SizedTexture::byInnerBox(sf::Texture const &texture, Vector2f const &innerBox) {
    if (isAreaWiderThanTexture(innerBox, texture))
        return byWidth(texture, innerBox.x);
    return byHeight(texture, innerBox.y);
}

SizedTexture::SizedTexture(sf::Texture const &sfTexture, float scale) :
        sfTexture(sfTexture),
        scale(scale) {
}

sf::Sprite SizedTexture::getSprite(sf::Vector2f const &position) const {
    auto sprite = sf::Sprite{};
    sprite.setPosition(position);
    applyScaleAndTexture(sprite);
    return sprite;
}

void SizedTexture::applyScaleAndTexture(sf::Sprite &sprite) const {
    sprite.setTexture(sfTexture, true);
    sprite.setScale(scale, scale);
}

sf::Vector2f SizedTexture::getSize() const {
    return static_cast<sf::Vector2f>(sfTexture.getSize()) * scale;
}

void SizedTexture::applyScaleAndTextureAndCenter(sf::Sprite &sprite, Vector2f const &center) const {
    applyScaleAndTexture(sprite);
    auto const size = Vector2u{sfTexture.getSize()}.cast<float>()*scale;
    sprite.setPosition(center - size / 2.f);
}
