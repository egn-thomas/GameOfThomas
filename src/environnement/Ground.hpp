#ifndef GROUND_HPP
#define GROUND_HPP

#include <SFML/Graphics.hpp>

class Ground {
private:
    sf::RectangleShape shape;

public:
    Ground(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);

    const sf::FloatRect getBounds() const;
};

#endif