#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include "Globals.hpp"

class Subtarget {
public:
    explicit Subtarget(sf::RenderTarget &target, sf::Rect<unsigned> const &area);

    ND sf::Vector2u getSize() const;

    void draw(sf::Drawable const &drawable);

private:
    sf::RenderTarget &target;
    sf::Rect<unsigned> area;
    ND sf::View getView() const;
};
