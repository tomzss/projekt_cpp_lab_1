#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Nodiscard.hpp"

/// Represents Texture of fixed size (keeping proportions)
class SizedTexture{
public:
    /// ==== create SizedTexture ====

    /// Texture scaled to fit provided width
    static SizedTexture byWidth(sf::Texture const&, float);

    /// Texture scaled to fit provided height
    static SizedTexture byHeight(sf::Texture const&, float);

    /// Texture scaled to be small enough to fit in provided area
    static SizedTexture byOuterBox(sf::Texture const&, sf::Vector2f const&);

    /// Texture scaled to be big enough to be able to cover provided area
    static SizedTexture byInnerBox(sf::Texture const&, sf::Vector2f const&);

    /// ==== get data ====

    /// Size of texture after scaling
    ND sf::Vector2f getSize() const;


    /// ==== render SizedTexture ====

    /// Generates default sprite of this texture; scales is and sets in provided position
    ND sf::Sprite getSprite(sf::Vector2f const &position) const;

    /// Scales sprite and sets it's texture. Other properties are unchanged
    void applyScaleAndTexture(sf::Sprite&) const;

private:
    /// Constructor is private; use one of byXXX functions
    SizedTexture(sf::Texture const &sfTexture, float scale);

    sf::Texture const& sfTexture;
    float scale;
};
