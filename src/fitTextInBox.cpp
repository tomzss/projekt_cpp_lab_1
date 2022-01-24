#include "fitTextInBox.hpp"

#include "sfml/Rect.hpp"

bool isAreaWiderThanText(sf::Vector2f const &area, sf::Text const &text) {
    auto const areaRatio = area.x / area.y;
    auto const textBounds = Rect{text.getGlobalBounds()};
    auto const textSize = textBounds.size();
    auto const textRatio = textSize.x / textSize.y;
    return areaRatio > textRatio;
}

void scaleToHeight(sf::Text &text, float height) {
    auto const &scale = height / static_cast<float>(text.getLocalBounds().height);
    text.setScale(scale, scale);
}

void scaleToWidth(sf::Text &text, float width) {
    auto const &scale = width / static_cast<float>(text.getLocalBounds().width);
    text.setScale(scale, scale);
}

void fitTextInBox(sf::Text &text, Vector2f const &boxSize) {
    if (isAreaWiderThanText(boxSize, text))
        return scaleToHeight(text, boxSize.y);
    return scaleToWidth(text, boxSize.x);
}

/// if you call it with fitTextInBox
/// first call fitTextInBox, then setTextPosition
void setTextPosition(sf::Text &text, Vector2f const &position) {
    auto const additionalHeight = text.getGlobalBounds().top;
    text.setPosition(position - Vector2{0.f, additionalHeight});
}
