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
    /**
     * @brief Constructeur d'un sol texturé
     * @param x Position X du sol
     * @param y Position Y du sol
     * @param width Largeur du sol
     * @param height Hauteur du sol
     * @param texture Texture à appliquer (répétée automatiquement)
     */
    TexturedGround(float x, float y, float width, float height, const sf::Texture& texture);

    void draw(sf::RenderWindow& window) override;

private:
    sf::RectangleShape texturedShape;
    sf::Texture texture;
};
