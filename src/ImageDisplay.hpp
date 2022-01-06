#pragma once

#include <deque>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Image.hpp"

using Images = std::deque<Image>;
using ImageIter = Images::const_iterator;

/// Images displayed on screen as gallery
/// Ordered in grid
class ImageGallery {
public:
    /// Images to display
    ND Images const &getImages() const;

    /// Area on screen do display images on (in pixels)
    ND sf::FloatRect const &getScreenArea() const;

    /// Number of columns in grid of images
    ND unsigned int getColumns() const;

    /// Distance between images (in pixels)
    ND float getMargin() const;

private:
    sf::FloatRect screenArea;
    unsigned columns;
    float margin;
    Images images;
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

    void drawSprite(sf::RenderTarget &target) {
    }

    std::optional<sf::Sprite> nextSprite(float spriteWidth) {
        if (iter++ == gallery.getImages().end())
            return std::nullopt;

        auto sprite = sf::Sprite{};
        current_position += sf::Vector2f{gallery.};

        sprite.setPosition(current_position);
        sprite.setTexture(iter->getTexture(), true);


        return sprite;
    }

    explicit ImageDisplay(ImageGallery const &gallery_);

private:
    ImageGallery const &gallery;
    ImageIter iter;
    sf::Vector2f current_position;
};
