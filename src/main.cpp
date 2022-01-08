#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <deque>
#include <SFML/Window/Event.hpp>
#include "Tag.hpp"
#include "Image.hpp"
#include "ImageLoader.hpp"
#include "ImageDisplay.hpp"

int main() {
    auto window = sf::RenderWindow{{600, 400}, "Tag gallery"};
    window.setFramerateLimit(30);

    auto tags = std::deque<Tag>{};
    auto images = std::deque<Image>{};

    auto loader = ImageLoader{fsys::directory_iterator{"/home/tomzs/temp"}};
    while (auto image = loader.nextImage())
        images.emplace_back(std::move(*image));
    for (auto const &image: images)
        std::cout << "loaded file: " << image.getPath().filename().string() << '\n';

    auto scrollpos = 0.f;
    auto constexpr scrollSpeed = 5.f;

    auto gallery = ImageGallery{images, {{0, 0}, static_cast<sf::Vector2f>(window.getSize())}, 3, 5};
    while (window.isOpen()) {
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::MouseWheelScrolled:
                    scrollpos += event.mouseWheelScroll.delta * scrollSpeed;
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
        window.display();
    }
}
