#include "Button.hpp"

#include <utility>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Subtarget.hpp"
#include "fitTextInBox.hpp"
#include "SizedTexture.hpp"

auto constexpr gap = 5.f;

Button::Button(sf::Font const &font, sf::String const &label, Rect<unsigned> area_, sf::Texture const &iconTexture,
               sf::Color iconColor, std::function<void()> callback) :
        area{area_},
        callback{std::move(callback)},
        underPress{false},
        iconTexture{iconTexture},
        iconColor(iconColor) {
    text.setFont(font);
    text.setCharacterSize(50);
    text.setString(label);
    updateText();
}

void Button::mousePressed(Vector2u const &mousePos) {
    if (area.contains(mousePos))
        underPress = true;
}

void Button::mouseReleased(Vector2u const &mousePos) {
    if (underPress and area.contains(mousePos))
        callback();
    underPress = false;
}

void drawRect(Subtarget &target, sf::Color const &color, Rect<float> const &rect) {
    auto shape = sf::RectangleShape{};
    shape.setPosition(rect.position());
    shape.setSize(rect.size());
    shape.setFillColor(color);
    target.draw(shape);
}

void Button::draw(sf::RenderTarget &sfTarget) const {
    auto target = Subtarget{sfTarget, area};
    auto shape = sf::RectangleShape{};
    shape.setSize(area.size().cast<float>());

    if (underPress)
        shape.setFillColor(sf::Color{90, 200, 200});
    else
        shape.setFillColor(sf::Color{60, 150, 150});

    auto icon = sf::Sprite{};
    icon.setColor(iconColor);
    SizedTexture::byHeight(iconTexture, area.height()).applyScaleAndTexture(icon);
    icon.setPosition({gap, 0});

    target.draw(shape);
    target.draw(icon);
    target.draw(text);
}

void Button::setArea(Rect<unsigned> const &newArea) {
    area = newArea;
    updateText();
}

void Button::updateText() {
    auto const iconWidth = sizedTexture().getSize().x;
    auto textAreaSize = area.size().cast<float>() - Vector2f{iconWidth, 0};

    auto const gapVec = Vector2f{gap, gap};
    text.setPosition({0, 0});
    text.setOrigin(Rect{text.getGlobalBounds()}.position());
    fitTextInBox(text, textAreaSize - gapVec * 2.f);
    auto const textSize = Rect{text.getGlobalBounds()}.size();
    text.setPosition(Vector2f{iconWidth, 0} + (textAreaSize - textSize + gapVec) / 2.f);
}

SizedTexture Button::sizedTexture() const {
    return SizedTexture::byHeight(iconTexture, area.height());
}
