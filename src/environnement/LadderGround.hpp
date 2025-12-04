#ifndef LADDERGROUND_HPP
#define LADDERGROUND_HPP
#pragma once
#include "Ground.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class LadderGround : public Ground
{
private:
    const sf::Texture* texturePtr = nullptr;

    std::vector<sf::Sprite> tiles;
    
    sf::RectangleShape texturedShape;
public:
    LadderGround(float x, float y, float width, float height, const sf::Texture& texture);
    ~LadderGround();

    void draw(sf::RenderWindow& window) override;

    bool isLadder() const { return true;}

    static sf::Texture* getDefaultTexture();
};
#endif