#pragma once

#include <SFML/System/Vector2.hpp>
#include "../Nodiscard.hpp"

template<typename T>
class Vector2 : public sf::Vector2<T> {
    using Parent = sf::Vector2<T>;
public:
    Vector2(T x, T y);

    template<typename U>
    ND Vector2<U> cast() const;

    Vector2(sf::Vector2<T> const &v);
    ND operator sf::Vector2<T> const &() const;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

// calculations

template<typename T>
ND Vector2<T> operator*(Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator/(Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator+(Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator/(Vector2<T> const &a, T b);

// sf::Vector compatibility

template<typename T>
ND Vector2<T> operator+(sf::Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator+(Vector2<T> const &a, sf::Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator*(sf::Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator*(Vector2<T> const &a, sf::Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator/(sf::Vector2<T> const &a, Vector2<T> const &b);

template<typename T>
ND Vector2<T> operator/(Vector2<T> const &a, sf::Vector2<T> const &b);

#include "Vector2.inl"
