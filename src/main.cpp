#include <iostream>
#include <deque>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "Tag.hpp"
#include "Image.hpp"
#include "ImageLoader.hpp"
#include "ImageGallery.hpp"
#include "ImageViewer.hpp"

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

    auto constexpr scrollSpeed = 30.f;
    auto constexpr zoomSpeed = 30.f;
    auto constexpr minimumImageSize = 130u;

    auto const galleryPosition = Vector2u{0, 0};
    auto gallerySize = [&]() -> Vector2u { return {window.getSize().x / 2, window.getSize().y}; };
    auto const gap = 10u;
    auto viewPosition = [&]() -> Vector2u { return {gallerySize().x + gap, 0}; };
    auto viewSize = [&]() -> Vector2u { return {window.getSize().x / 2 - gap, window.getSize().y}; };
    auto columns = [&]() -> unsigned { return gallerySize().x / minimumImageSize; };
    auto gallery = ImageGallery{images, {galleryPosition, gallerySize()}, columns(), 5};

    while (window.isOpen()) {
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Resized: {
                    auto const size_u = Vector2u{event.size.width, event.size.height};
                    auto const size_f = size_u.cast<float>();
                    window.setView({size_f * 0.5f, size_f});
                    gallery.setArea({galleryPosition, {size_u.x / 2u, size_u.y}});
                    gallery.setColumnsCount(columns());
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                    gallery.scroll(event.mouseWheelScroll.delta * scrollSpeed);
                    break;
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.x < 0 or event.mouseButton.y < 0)
                        break;
                    auto const mousePos = Vector2i{event.mouseButton.x, event.mouseButton.y}.cast<unsigned>();
                    if (auto pos = gallery.globalPosToInAreaPos(mousePos))
                        gallery.pressed(*pos);
                    break;
                }
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        auto selectedImage = gallery.getSelectedImage();

        window.clear(sf::Color::Black);
        gallery.draw(window);
        if (selectedImage != nullptr) {
            auto selectedTexture = selectedImage->getTexture();
            auto imageViewer = ImageViewer{selectedTexture, {viewPosition(), viewSize()}};
            imageViewer.draw(window);
        }
        window.display();
    }
}
