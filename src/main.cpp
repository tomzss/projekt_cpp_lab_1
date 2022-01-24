#include <iostream>
#include <deque>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "Tag.hpp"
#include "Image.hpp"
#include "ImageGallery.hpp"
#include "ImageViewer.hpp"
#include "Button.hpp"
#include "Directory.hpp"
#include "fitTextInBox.hpp"

fsys::path getDefaultPath(int argc, char **args) {
    if (argc > 1)
        return args[1];

    auto homePath = fsys::path{std::getenv("HOME")};
    if (is_directory(homePath))
        return homePath;

    return fsys::current_path().root_path();
}

int main(int argc, char **args) {
    auto font = sf::Font{};
    if (not font.loadFromFile("Manjari-Regular.otf")) {
        std::cerr << "FAILED TO LOAD FONT ABORDING ABORDING AAAAAAAAAAA";
        return EXIT_FAILURE;
    }

    auto const startPath = getDefaultPath(argc, args);

    /// make window
    auto window = sf::RenderWindow{{700, 400}, "Gallery in " + startPath.string()};
    window.setFramerateLimit(30);

    /// main data containers
    auto tags = std::deque<Tag>{};
    auto directory = Directory{startPath};

    /// input sensitivity
    auto constexpr scrollSpeed = 30.f;
    auto constexpr zoomSpeed = 30.f;

    /// layout
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

    auto gallery = ImageGallery{directory.getImages(), galleryArea(), columns(), 5};

    auto buttonsArea = [&]() -> Rect<unsigned> {
        auto position = viewArea().position() + Vector2u{0, viewArea().size().y} + Vector2u{gap, gap};
        auto size = Vector2u{viewArea().size().x - 2 * gap, viewArea().size().y};
        return {position, size};
    };

    auto buttonArea = [&](unsigned i) -> Rect<unsigned> {
        auto size = Vector2u{buttonsArea().size().x, 30};
        auto position = buttonsArea().position() + Vector2u{0, (size.y + gap) * i};
        return {position, size};
    };

    auto makeButtons = [&](auto&callback) -> std::deque<Button> {
        auto buttons = std::deque<Button>{};
        auto i = std::size_t{0};
        for (auto const &dir: directory.getAvailableDirectories()) {
            buttons.emplace_back(
                    font, dir.filename().string(), buttonArea(i), callback);
            i++;
        }
        return buttons;
    };

    auto directoriesButtons =std::deque<Button>{};

    auto buttonCallback = [&](auto dir) {
        directory = Directory{dir};
        directoriesButtons = makeButtons(buttonCallback);
    };

    directoriesButtons=makeButtons(buttonCallback);
    /// main loop
    while (window.isOpen()) {
        /// event processing
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Resized: {
                    auto const size_u = Vector2u{event.size.width, event.size.height};
                    auto const size_f = size_u.cast<float>();
                    window.setView({size_f * 0.5f, size_f});
                    gallery.setArea({galleryArea()});
                    gallery.setColumnsCount(columns());
                    directoriesButtons = makeButtons(buttonCallback);
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
                    for (auto &button: directoriesButtons)
                        button.mousePressed(mousePos);
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.x < 0 or event.mouseButton.y < 0)
                        break;
                    auto const mousePos = Vector2i{event.mouseButton.x, event.mouseButton.y}.cast<unsigned>();
                    for (auto &button: directoriesButtons)
                        button.mouseReleased(mousePos);
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

        /// drawing
        window.clear(sf::Color::Black);
        gallery.draw(window);
        if (selectedImage != nullptr) {
            auto selectedTexture = selectedImage->getTexture();
            auto imageViewer = ImageViewer{selectedTexture, viewArea()};
            imageViewer.draw(window);
        }
        for (auto &button: directoriesButtons)
            button.draw(window);
        window.display();
    }
}
