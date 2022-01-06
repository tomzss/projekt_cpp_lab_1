#include <iostream>

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <deque>
#include <optional>
#include <utility>

#define ND [[nodiscard]]
namespace fsys = std::filesystem;

class Tag {
public:
    ND std::string_view getName() const {
        return name;
    }

private:
    std::string name;
};

class Image {
public:
    using Tags = std::vector<std::reference_wrapper<Tag const>>;

    ND Tags const &getTags() {
        return tags;
    }

    void addTag(Tag const &tag) {
        tags.emplace_back(tag);
    }

    static std::optional<Image> tryLoad(fsys::path path) {
        auto texture = sf::Texture{};
        if (not texture.loadFromFile(path.string()))
            return std::nullopt;
        return Image{std::move(path), texture, {}};
    }

    ND fsys::path const &getPath() const {
        return path;
    }

    ND sf::Texture const &getTexture() const {
        return sfTexture;
    }

private:
    fsys::path path;
    sf::Texture sfTexture;
    Tags tags;

    Image(fsys::path path, sf::Texture const &sfTexture, Tags tags) :
            path{std::move(path)},
            sfTexture{sfTexture},
            tags{std::move(tags)} {
    }
};

class ImageLoader {
public:
    std::optional<Image> getNext() {
        if (iter == fsys::end(iter))
            return std::nullopt;
        auto oldIter = iter;
        ++iter;
        if (auto image = Image::tryLoad(*oldIter))
            return image;
        return getNext();
    }

    explicit ImageLoader(fsys::directory_iterator iter) :
            iter(std::move(iter)) {
    }

private:
    fsys::directory_iterator iter;
};

class ImageDisplay {
public:
    void draw(sf::RenderTarget &target) {
        auto sprite = sf::Sprite{};
        sprite.setPosition(screenArea.height, screenArea.width);
        for (auto const &image: images) {
            sprite.setTexture(image.getTexture(), true);
            float targetWidth = (screenArea.width + margin) / static_cast<float>(columns);
            float scale = targetWidth / static_cast<float>(image.getTexture().getSize().x);
            sprite.setScale(scale, scale);
            auto const &bounds = sprite.getGlobalBounds();
            if (bounds.top + bounds.height > screenArea.top + screenArea.height)
                break;
            target.draw(sprite);
            sprite.setPosition(sprite.getPosition() + sf::Vector2f(margin, margin + targetWidth));
        }
    }

private:
    void drawSprite(sf::RenderTarget &target) {

    }

    sf::FloatRect screenArea;
    unsigned columns;
    float margin;
    std::deque<Image> images;
};

int main() {
    auto window = sf::RenderWindow{{600, 400}, "Tag gallery"};
    window.setFramerateLimit(30);

    auto tags = std::deque<Tag>{};
    auto images = std::deque<Image>{};

    auto loader = ImageLoader{fsys::directory_iterator{"/home/tomzs/temp"}};
    while (auto image = loader.getNext())
        images.emplace_back(std::move(*image));
    for (auto const &image: images)
        std::cout << "loaded file: " << image.getPath().filename().string() << '\n';

    while (window.isOpen()) {
        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        window.clear(sf::Color::Black);
        auto sprite = sf::Sprite{};
        for (auto const &image: images) {
            sprite.setTexture(image.getTexture(), true);
            float targetHeight = 50;
            float scale = targetHeight / static_cast<float>(image.getTexture().getSize().y);
            sprite.setScale(scale, scale);
            window.draw(sprite);
            sprite.setPosition(sprite.getPosition() + sf::Vector2f(0, targetHeight));
        }
        window.display();
    }
}
