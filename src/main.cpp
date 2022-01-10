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
    auto window = sf::RenderWindow{{700, 400}, "Tag gallery"};
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

    auto const displayPosition = sf::Vector2u{0, 0};
    auto const displaySize = sf::Vector2u{window.getSize().x / 2, window.getSize().y};
    auto gallery = ImageGallery{images, {displayPosition, displaySize}, 3, 5};

    while (window.isOpen()) {
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Resized: {
                    auto const size_u = sf::Vector2u{event.size.width, event.size.height};
                    auto const size_f = static_cast<sf::Vector2f>(size_u);
                    window.setView({size_f * 0.5f, size_f});
                    gallery.setArea({displayPosition, {size_u.x / 2u, size_u.y}});
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                    gallery.scroll(event.mouseWheelScroll.delta * scrollSpeed);
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        window.clear(sf::Color::Black);
        gallery.draw(window);
        window.display();
    }
}
