#pragma once

#include <functional>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include "sfml/Rect.hpp"
#include "SizedTexture.hpp"

class Button {
public:
    Button(sf::Font const &font, sf::String const &label, Rect<unsigned> area_, sf::Texture const &iconTexture,
           sf::Color iconColor, std::function<void()> callback);

    void mousePressed(Vector2u const &mousePos);
    void mouseReleased(Vector2u const &mousePos);

    void setArea(Rect<unsigned> const &newArea);

    void draw(sf::RenderTarget &target) const;

private:
    Rect<unsigned> area;
    std::function<void()> callback;
    sf::Text text;
    bool underPress;

    sf::Texture const &iconTexture;
    sf::Color iconColor;

    void updateText();
    ND SizedTexture sizedTexture() const;
};
