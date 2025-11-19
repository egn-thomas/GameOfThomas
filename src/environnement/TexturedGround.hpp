#pragma once
#include "Ground.hpp"
#include "TexturedGround.hpp"
#include <SFML/Graphics.hpp>
#include <string>

#pragma once
#include "Ground.hpp"
#include <SFML/Graphics.hpp>

class TexturedGround : public Ground {
public:
    TexturedGround(float x, float y, float width, float height, const sf::Texture& texture);

    void draw(sf::RenderWindow& window) override;

    static sf::Texture* getDefaultTexture();

private:
    sf::RectangleShape texturedShape;
    sf::Texture texture;
};
