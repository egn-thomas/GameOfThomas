#include "TexturedGround.hpp"
#include <iostream>

/**
 * @brief Constructeur d'un sol texturé
 * @param x Position X du sol
 * @param y Position Y du sol
 * @param width Largeur du sol
 * @param height Hauteur du sol
 * @param texture Texture à appliquer (répétée automatiquement)
 * @note Il est important que la texture passée en paramètre ait son mode "repeated" activé via setRepeated(true).
 */
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

/**
 * @brief Dessine le sol texturé dans la fenêtre donnée.
 * @param window La fenêtre SFML où dessiner le sol.
 */
void TexturedGround::draw(sf::RenderWindow& window)
{
    window.draw(texturedShape);
}

sf::Texture* TexturedGround::getDefaultTexture()
{
    static sf::Texture defaultTexture;
    static bool isLoaded = false;

    if (!isLoaded)
    {
        if (!defaultTexture.loadFromFile("../src/assets/images/brick.png"))
        {
            std::cerr << "Erreur : impossible de charger ../src/assets/images/brick.png\n";
        }
        else
        {
            defaultTexture.setRepeated(true);
            isLoaded = true;
            std::cout << "Texture par défaut chargée avec succès\n";
        }
    }

    return &defaultTexture;
}
