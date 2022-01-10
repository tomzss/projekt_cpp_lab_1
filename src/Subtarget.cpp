#include "Subtarget.hpp"

sf::Vector2u Subtarget::getSize() const {
    return {area.width, area.height};
}

Subtarget::Subtarget(sf::RenderTarget &target, sf::Rect<unsigned> const &area) :
        area(area),
        target(target) {
}

void Subtarget::draw(sf::Drawable const &drawable) {
    auto const oldView = target.getView();
    target.setView(getView());
    target.draw(drawable);
    target.setView(oldView);
}

sf::View Subtarget::getView() const {
    auto const oldView = target.getView();
    auto const viewportSize = sf::Vector2f{static_cast<float>(area.width) / oldView.getSize().x,
                                           static_cast<float>(area.height) / oldView.getSize().y};
    auto const viewportPos = sf::Vector2f{static_cast<float>(area.left) / oldView.getSize().x,
                                          static_cast<float>(area.top) / oldView.getSize().y};
    auto newView = target.getView();
    newView.setViewport({viewportPos, viewportSize});
    newView.setSize(static_cast<sf::Vector2f> (getSize()));
    newView.setCenter({area.width / 2.f, area.height / 2.f});
    return newView;
}
