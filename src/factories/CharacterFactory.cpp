#include "CharacterFactory.hpp"
#include <iostream>

/**
 * @brief Crée un joueur avec des paramètres prédéfinis.
 *
 * @param windowSize La taille de la fenêtre pour positionner le joueur au centre.
 * @return std::unique_ptr<Player> Pointeur unique vers le joueur créé.
 */
std::unique_ptr<Player> CharacterFactory::createPlayer(const sf::Vector2u &windowSize)
{
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile("../src/assets/images/playerBaseAnimation.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
        return nullptr;
    }

    auto player = std::make_unique<Player>("Player", 100, 50, 500.f, texture);
    player->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    player->setAnimationParams(1, 32, 32, 1.f);
    player->setHitbox(5.f, 7.f, 17.f, 25.f);

    return player;
}

/**
 * @brief Crée une liste de personnages non-joueurs (PNJ) avec des paramètres prédéfinis.
 *
 * @param windowSize La taille de la fenêtre pour positionner les PNJ.
 * @param offset Un décalage à appliquer à la position des PNJ.
 * @return std::vector<std::unique_ptr<NonPlayer>> Vecteur de pointeurs uniques vers les PNJ créés.
 */
std::vector<std::unique_ptr<NonPlayer>> CharacterFactory::createNonPlayer(
    const sf::Vector2u &windowSize,
    const sf::Vector2f &offset)
{
    std::vector<std::unique_ptr<NonPlayer>> npcs;
    auto pnjTexture = std::make_shared<sf::Texture>();
    if (!pnjTexture->loadFromFile("../src/assets/images/nonPlayerBaseAnimation.png"))
    {
        std::cerr << "Failed to load nonPlayer texture!" << std::endl;
        return {};
    }

    // PNJ 1
    auto pnj1 = std::make_unique<NonPlayer>("Enemy1", 50, 10, 450.f, pnjTexture);
    pnj1->setPosition(windowSize.x / 3.f, windowSize.y / 2.f);
    pnj1->setAnimationParams(4, 32, 32, 6.f);
    pnj1->setHitbox(0.f, 0.f, 32.f, 32.f);
    npcs.push_back(std::move(pnj1));

    // PNJ 2
    auto pnj2 = std::make_unique<NonPlayer>("Enemy2", 60, 15, 420.f, pnjTexture);
    pnj2->setPosition(windowSize.x / 2.f, windowSize.y / 1.8f);
    pnj2->setAnimationParams(4, 32, 32, 6.f);
    pnj2->setHitbox(0.f, 0.f, 32.f, 32.f);
    npcs.push_back(std::move(pnj2));

    return npcs;
}
