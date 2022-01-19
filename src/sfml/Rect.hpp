#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "../Nodiscard.hpp"
#include "Vector2.hpp"

template<typename T>
class Rect : public sf::Rect<T> {
    using Parent = sf::Rect<T>;
public:
    ND T top() const;
    ND T bottom() const;
    ND T left() const;
    ND T right() const;

    ND Vector2<T> leftTop() const;
    ND Vector2<T> rightTop() const;
    ND Vector2<T> leftBottom() const;
    ND Vector2<T> rightBottom() const;

    ND T width() const;
    ND T height() const;

    ND Vector2<T> position() const;
    ND Vector2<T> size() const;

    template<typename U>
    ND Rect<U> cast() const;

    Rect(sf::Rect<T> const &);
    Rect(T left, T top, T width, T height);
    Rect(Vector2<T> const &position, Vector2<T> const &size);

    ND operator sf::Rect<T> const &() const;
    ND operator sf::Rect<T> const () const;
    ND operator sf::Rect<T> ();
};

#include "Rect.inl"
