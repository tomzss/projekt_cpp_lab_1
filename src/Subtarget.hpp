#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include "sfml/Rect.hpp"
#include "Nodiscard.hpp"

/// Part of sf::RenderTarget narrowed to certain area.
/// Has own coordinate system and no access to target outside provided area.
class Subtarget {
public:
    explicit Subtarget(sf::RenderTarget &target, Rect<unsigned> const &area);

    ND Vector2u getSize() const;

    /// Drawings will be cropped to fit into this area.
    void draw(sf::Drawable const &drawable);

private:
    sf::RenderTarget &target;
    Rect<unsigned> area;
    ND sf::View getView() const;
};
