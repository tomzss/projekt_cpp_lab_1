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
#include "Button.hpp"

int main() {
    auto font = sf::Font{};
    if (not font.loadFromFile("Manjari-Regular.otf")) {
        std::cerr << "FAILED TO LOAD FONT ABORDING ABORDING AAAAAAAAAAA";
        return EXIT_FAILURE;
    }

    // make window
    auto window = sf::RenderWindow{{700, 400}, "Gallery"};
    window.setFramerateLimit(30);

    // main data containers
    auto tags = std::deque<Tag>{};
    auto images = std::deque<Image>{};

    // load images and test ImageLoader
    auto loader = ImageLoader{fsys::directory_iterator{"/home/tomzs/temp"}};
    while (auto image = loader.nextImage())
        images.emplace_back(std::move(*image));
    for (auto const &image: images)
        std::cout << "loaded file: " << image.getPath().filename().string() << '\n';

    // input sensitivity
    auto constexpr scrollSpeed = 30.f;
    auto constexpr zoomSpeed = 30.f;

    // layout
    auto constexpr minimumImageSize = 130u;
    auto constexpr gap = 10u;

    auto galleryArea = [&]() -> Rect<unsigned> {
        auto const position = Vector2u{0, 0};
        auto const size = Vector2u{window.getSize().x / 2, window.getSize().y};
        return {position, size};
    };

    auto viewArea = [&]() -> Rect<unsigned> {
        auto const position = Vector2u{galleryArea().size().x + gap, 0};
        auto const size = Vector2u{window.getSize().x, window.getSize().y} / 2u - Vector2u{gap, gap};
        return {position, size};
    };

    auto columns = [&]() -> unsigned { return galleryArea().size().x / minimumImageSize; };

    auto gallery = ImageGallery{images, galleryArea(), columns(), 5};

    auto testButtonArea = [&]() -> Rect<unsigned> {
        auto position = viewArea().position() + Vector2u{0, viewArea().size().y} + Vector2u{gap, gap};
        auto size = Vector2u{viewArea().size().x - 2 * gap, 30};
        return {position, size};
    };

    auto testButton = Button{font, "TESTING BUTTON", testButtonArea(), [] { std::cout << "A" << std::endl; }};

    // main loop
    while (window.isOpen()) {
        // event processing
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Resized: {
                    auto const size_u = Vector2u{event.size.width, event.size.height};
                    auto const size_f = size_u.cast<float>();
                    window.setView({size_f * 0.5f, size_f});
                    gallery.setArea({galleryArea()});
                    gallery.setColumnsCount(columns());
                    testButton.setArea(testButtonArea());
                    break;
                }
                case sf::Event::MouseWheelScrolled: {
                    if (event.mouseWheelScroll.x < 0 or event.mouseWheelScroll.y < 0)
                        break;
                    if (galleryArea().contains(
                            Vector2i{event.mouseWheelScroll.x, event.mouseWheelScroll.y}.cast<unsigned>()))
                        gallery.scroll(event.mouseWheelScroll.delta * scrollSpeed);
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.x < 0 or event.mouseButton.y < 0)
                        break;
                    auto const mousePos = Vector2i{event.mouseButton.x, event.mouseButton.y}.cast<unsigned>();
                    if (auto pos = gallery.globalPosToInAreaPos(mousePos))
                        gallery.pressed(*pos);
                    testButton.mousePressed(mousePos);
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.x < 0 or event.mouseButton.y < 0)
                        break;
                    auto const mousePos = Vector2i{event.mouseButton.x, event.mouseButton.y}.cast<unsigned>();
                    testButton.mouseReleased(mousePos);
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

        // drawing
        window.clear(sf::Color::Black);
        gallery.draw(window);
        testButton.draw(window);
        if (selectedImage != nullptr) {
            auto selectedTexture = selectedImage->getTexture();
            auto imageViewer = ImageViewer{selectedTexture, viewArea()};
            imageViewer.draw(window);
        }
        window.display();
    }
}
