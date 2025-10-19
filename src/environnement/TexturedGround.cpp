#include "TexturedGround.hpp"
#include <iostream>

TexturedGround::TexturedGround(float x, float y, float width, float height, const sf::Texture& texture)
    : Ground(x, y, width, height)
{
    texturedShape.setSize(sf::Vector2f(width, height));
    texturedShape.setPosition(x, y);

    // Applique la texture répétée
    texturedShape.setTexture(&texture);
    texturedShape.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));

    // Petit contour gris béton
    texturedShape.setOutlineThickness(2.f);
    texturedShape.setOutlineColor(sf::Color(100, 100, 100));
}

void TexturedGround::draw(sf::RenderWindow& window)
{
    window.draw(texturedShape);
}
