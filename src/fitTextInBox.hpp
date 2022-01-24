#pragma once

#include <SFML/Graphics/Text.hpp>
#include "sfml/Vector2.hpp"

void fitTextInBox(sf::Text &text, Vector2f const &boxSize);

void setTextPosition(sf::Text &text, Vector2f const &position);
