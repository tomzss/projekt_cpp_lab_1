#include <iostream>
#include <deque>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "Tag.hpp"
#include "Image.hpp"
#include "ImageLoader.hpp"
#include "ImageDisplay.hpp"

int main() {
    auto window = sf::RenderWindow{{600, 900}, "Tag gallery"};
    window.setFramerateLimit(30);

    auto tags = std::deque<Tag>{};
    auto images = std::deque<Image>{};

    auto loader = ImageLoader{fsys::directory_iterator{"/home/tomzs/temp"}};
    while (auto image = loader.nextImage())
        images.emplace_back(std::move(*image));
    for (auto const &image: images)
        std::cout << "loaded file: " << image.getPath().filename().string() << '\n';

    auto scrollpos = 0.f;
    auto constexpr scrollSpeed = 30.f;

    auto const displaySize = sf::Vector2f(window.getSize().x, window.getSize().y - 400);
    auto const displayPosition = sf::Vector2f{0,100};

    auto frame = sf::RectangleShape{};
    frame.setFillColor(sf::Color::Transparent);
    frame.setSize(displaySize);
    frame.setPosition(displayPosition);
    frame.setOutlineThickness(5);
    frame.setOutlineColor(sf::Color::Red);

    auto gallery = ImageGallery{images, {displayPosition, displaySize}, 3, 5};
    while (window.isOpen()) {
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::MouseWheelScrolled:
                    scrollpos -= event.mouseWheelScroll.delta * scrollSpeed;
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        window.clear(sf::Color::Black);
        gallery.draw(window, scrollpos);
        window.draw(frame);
        window.display();
    }
}
