#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "sfml/Rect.hpp"

class ImageViewer{
public:
    ImageViewer(sf::Texture const&, Rect<unsigned> const& area);

    void draw(sf::RenderTarget&target);

private:
    sf::Texture const& sfTexture;
    Rect<unsigned> screenArea;
};
