#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include "sfml/Rect.hpp"
#include "Nodiscard.hpp"

class Subtarget {
public:
    explicit Subtarget(sf::RenderTarget &target, Rect<unsigned> const &area);

    ND Vector2u getSize() const;

    void draw(sf::Drawable const &drawable);

private:
    sf::RenderTarget &target;
    Rect<unsigned> area;
    ND sf::View getView() const;
};
