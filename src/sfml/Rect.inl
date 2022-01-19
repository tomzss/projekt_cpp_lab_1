#include "Rect.hpp"

template<typename T>
T Rect<T>::top() const {
    return Parent::top;
}

template<typename T>
T Rect<T>::bottom() const {
    return Parent::top + Parent::height;
}

template<typename T>
T Rect<T>::left() const {
    return Parent::left;
}

template<typename T>
T Rect<T>::right() const {
    return Parent::left + Parent::width;
}

template<typename T>
Vector2<T> Rect<T>::leftTop() const {
    return {left(), top()};
}

template<typename T>
Vector2<T> Rect<T>::rightTop() const {
    return {right(), top()};
}

template<typename T>
Vector2<T> Rect<T>::leftBottom() const {
    return {left(), bottom()};
}

template<typename T>
Vector2<T> Rect<T>::rightBottom() const {
    return {right(), bottom()};
}

template<typename T>
Vector2<T> Rect<T>::size() const {
    return {Parent::width, Parent::height};
}

template<typename T>
Rect<T>::Rect(T left_, T top_, T width_, T height_):
        sf::Rect<T>{left_, top_, width_, height_} {
}

template<typename T>
Rect<T>::Rect(Vector2<T> const &position_, Vector2<T> const &size_):
        sf::Rect<T>{position_, size_} {
}

template<typename T>
Rect<T>::Rect(sf::Rect<T> const &r):
        sf::Rect<T>{r} {
}

template<typename T>
Vector2<T> Rect<T>::position() const {
    return {left(), top()};
}

template<typename T>
T Rect<T>::width() const {
    return Parent::width;
}

template<typename T>
T Rect<T>::height() const {
    return Parent::height;
}

template<typename T>
Rect<T>::operator sf::Rect<T> const() const {
    return static_cast<Parent const>(*this);
}

template<typename T>
Rect<T>::operator sf::Rect<T> const &() const {
    return static_cast<Parent const &>(*this);
}

template<typename T>
Rect<T>::operator sf::Rect<T>() {
    return sf::Rect<T>();
}

template<typename T>
template<typename U>
Rect<U> Rect<T>::cast() const {
    return sf::Rect<U>{static_cast<U>(Parent::left), static_cast<U>(Parent::top), static_cast<U>(Parent::width),
                       static_cast<U>(Parent::height)};
}
