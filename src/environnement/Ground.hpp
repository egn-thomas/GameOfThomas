#pragma once
#include <SFML/Graphics.hpp>

class Ground {
public:
    Ground(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);
    
    const sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
};