#include "Ground.hpp"

Ground::Ground(float x, float y, float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(x, y);
}

void Ground::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

const sf::FloatRect Ground::getBounds() const {
    return shape.getGlobalBounds();
}

sf::RectangleShape Ground::getShape() {
    return shape;
}
