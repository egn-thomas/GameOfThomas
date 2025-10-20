#include "GroundFactory.hpp"
#include "../environnement/TexturedGround.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

/**
 * @brief Crée une série de plateformes par défaut pour le niveau de jeu.
 * 
 * @param windowSize La taille de la fenêtre de jeu pour positionner les plateformes.
 * @return std::vector<std::unique_ptr<Ground>> Un vecteur de pointeurs
 */
std::vector<std::unique_ptr<Ground>> GroundFactory::createDefaultGrounds(const sf::Vector2u &windowSize)
{
    std::vector<std::unique_ptr<Ground>> grounds;

    static sf::Texture brickTexture;
    static bool textureLoaded = false;

    if (!textureLoaded)
    {
        if (!brickTexture.loadFromFile("../src/assets/images/brick.png"))
            std::cerr << "Erreur : impossible de charger ../src/assets/images/brick.png\n";
        else
        {
            brickTexture.setRepeated(true);
            textureLoaded = true;
            std::cout << "Texture brique chargée avec succès\n";
        }
    }

    float w = static_cast<float>(windowSize.x);
    float h = static_cast<float>(windowSize.y);

    grounds.push_back(std::make_unique<TexturedGround>(0.f, h - 50.f, w, 50.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(50.f, 900.f, 500.f, 10.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(600.f, 750.f, 500.f, 50.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(100.f, 600.f, 500.f, 50.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(600.f, 450.f, 500.f, 10.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(100.f, 300.f, 500.f, 10.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(1300.f, 900.f, 100.f, 250.f, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(0.f, 0.f, 50.f, h, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(w - 50.f, 0.f, 50.f, h, brickTexture));
    grounds.push_back(std::make_unique<TexturedGround>(0.f, 0.f, w, 50.f, brickTexture));

    return grounds;
}