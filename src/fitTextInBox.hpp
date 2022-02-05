#pragma once

#include <SFML/Graphics/Text.hpp>
#include "sfml/Vector2.hpp"

/// Scale text to be lo larger than boxSize.
/// Keeps ratio.
void fitTextInBox(sf::Text &text, Vector2f const &boxSize);
