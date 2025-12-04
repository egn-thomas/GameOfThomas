#include "LadderGround.hpp"
#include <iostream>

/**
 * @brief Constructeur d'un sol échelle
 * @param x Position X du sol
 * @param y Position Y du sol
 * @param width Largeur du sol
 * @param height Hauteur du sol
 * @param texture Texture à appliquer (répétée automatiquement)
 * @note Il est important que la texture passée en paramètre ait son mode "repeated" activé via setRepeated(true).
 */
LadderGround::LadderGround(float x, float y, float width, float height, const sf::Texture &texture)
    : Ground(x, y, width, height), texturePtr(&texture)
{
    int texW = texturePtr->getSize().x;
    int texH = texturePtr->getSize().y;

    std::cout << "LadderGround: texture size = " << texW << "x" << texH << "\n";

    const_cast<sf::Texture &>(texture).setRepeated(true);

    texturedShape.setPosition(x, y);
    texturedShape.setSize(sf::Vector2f(width, height));

    texturedShape.setTexture(&texture);

    texturedShape.setTextureRect(sf::IntRect(0, 0, width, height));

    this->isSolid = false;
}

LadderGround::~LadderGround() = default;

/**
 * @brief Dessine le sol échelle dans la fenêtre donnée.
 * @param window La fenêtre SFML où dessiner le sol.
 */
void LadderGround::draw(sf::RenderWindow &window)
{
    if (!tiles.empty())
    {
        for (auto &s : tiles)
            window.draw(s);
    }
    else
    {
        window.draw(texturedShape);
    }
}

sf::Texture *LadderGround::getDefaultTexture()
{
    static sf::Texture defaultTexture;
    static bool isLoaded = false;

    if (!isLoaded)
    {
        if (!defaultTexture.loadFromFile("../src/assets/images/ladder.png"))
        {
            std::cerr << "Erreur : impossible de charger ../src/assets/images/ladder.png\n";
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