#ifndef GROUND_HPP
#define GROUND_HPP

#include <SFML/Graphics.hpp>

class Ground {
protected:
    sf::RectangleShape shape;

    bool isSolid = true;

public:
    Ground(float x, float y, float width, float height);

    virtual void draw(sf::RenderWindow& window);

    const sf::FloatRect getBounds() const;

    sf::RectangleShape getShape();

    bool isGroundSolid() const;
    // By default a ground is not a ladder. Override in LadderGround.
    virtual bool isLadder() const { return false; }
};

#endif