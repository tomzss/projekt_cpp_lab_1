template<typename T>
Vector2<T> operator*(Vector2<T> const &a, Vector2<T> const &b) {
    return {a.x * b.x, a.y * b.y};
}

template<typename T>
Vector2<T> operator/(Vector2<T> const &a, Vector2<T> const &b) {
    return {a.x / b.x, a.y / b.y};
}

template<typename T>
template<typename U>
Vector2<U> Vector2<T>::cast() const {
    return {static_cast<U>(Parent::x), static_cast<U>(Parent::y)};
}

template<typename T>
Vector2<T>::operator sf::Vector2<T> const &() const {
    return static_cast<Parent const &>(*this);
}

template<typename T>
Vector2<T>::Vector2(sf::Vector2<T> const &v) :
        sf::Vector2<T>{v} {
}

template<typename T>
Vector2<T>::Vector2(T x, T y) :
        sf::Vector2<T>{x, y} {
}

template<typename T>
ND Vector2<T> operator*(sf::Vector2<T> const &a, Vector2<T> const &b) {
    return Vector2<T>{a} * b;
}

template<typename T>
ND Vector2<T> operator*(Vector2<T> const &a, sf::Vector2<T> const &b) {
    return a * Vector2<T>{b};
}

template<typename T>
ND Vector2<T> operator/(sf::Vector2<T> const &a, Vector2<T> const &b) {
    return Vector2<T>{a} / b;
}

template<typename T>
ND Vector2<T> operator/(Vector2<T> const &a, sf::Vector2<T> const &b) {
    return a / Vector2<T>{b};
}

template<typename T>
ND Vector2<T> operator+(sf::Vector2<T> const &a, Vector2<T> const &b) {
    return Vector2<T>{a} + b;
}

template<typename T>
ND Vector2<T> operator+(Vector2<T> const &a, Vector2<T> const &b){
    return sf::Vector2<T>{a} + sf::Vector2<T>{b};
}

template<typename T>
ND Vector2<T> operator-(sf::Vector2<T> const &a, Vector2<T> const &b) {
    return Vector2<T>{a} - b;
}

template<typename T>
ND Vector2<T> operator+(Vector2<T> const &a, sf::Vector2<T> const &b) {
    return a + Vector2<T>{b};
}

template<typename T>
bool operator==(Vector2<T> const &a, Vector2<T> const &b) {
    return a.x == b.x and a.y == b.y;
}

template<typename T>
bool operator!=(Vector2<T> const &a, Vector2<T> const &b) {
    return not a == b;
}





//





template<typename T>
ND Vector2<T> operator-(Vector2<T> const &a, sf::Vector2<T> const &b) {
    return a - Vector2<T>{b};
}
