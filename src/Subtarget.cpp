#include "Subtarget.hpp"

Vector2u Subtarget::getSize() const {
    return area.size();
}

Subtarget::Subtarget(sf::RenderTarget &target_, Rect<unsigned> const &area_) :
        area(area_),
        target(target_) {
}

void Subtarget::draw(sf::Drawable const &drawable) {
    auto const oldView = target.getView();
    target.setView(getView());
    target.draw(drawable);
    target.setView(oldView);
}

sf::View Subtarget::getView() const {
    auto const oldView = target.getView();
    auto const viewportSize = sf::Vector2f{area.size().cast<float>() / oldView.getSize()};
    auto const viewportPos = sf::Vector2f{area.position().cast<float>() / oldView.getSize()};
    auto newView = target.getView();
    newView.setViewport({viewportPos, viewportSize});
    newView.setSize(static_cast<sf::Vector2f> (getSize()));
    newView.setCenter(area.size().cast<float>() / 2.f);
    return newView;
}
