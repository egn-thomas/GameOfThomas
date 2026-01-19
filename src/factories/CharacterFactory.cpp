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
    auto textureIdle = std::make_shared<sf::Texture>();
    if (!textureIdle->loadFromFile("../src/assets/images/playerIdleAnimation.png"))
    {
        std::cerr << "Failed to load player texture! Using empty fallback texture." << std::endl;
        // create a small empty texture as a safe fallback so the game can run without assets
        textureIdle->create(32, 32);
    }
    auto textureWalk = std::make_shared<sf::Texture>();
    if (!textureWalk->loadFromFile("../src/assets/images/playerWalkAnimation.png"))
    {
        std::cerr << "Failed to load player walk texture! Using empty fallback texture." << std::endl;
        textureWalk->create(32, 32);
    }

    auto player = std::make_unique<Player>("Player", 100, 50, 250.f, textureIdle);
    player->setAnimationTexture(AnimationState::Idle, textureIdle, 1, 32, 32, 1.f);
    player->setAnimationTexture(AnimationState::Walk, textureWalk, 4, 32, 32, 6.f);
    player->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    player->setHitbox(5.f, 7.f, 17.f, 25.f);

    return player;
}

/**
 * @brief Crée une liste de chandelles (ennemis IA) avec des paramètres prédéfinis.
 *
 * @param windowSize La taille de la fenêtre pour positionner les ennemis.
 * @param offset Un décalage à appliquer à la position des ennemis.
 * @return std::vector<std::unique_ptr<NonPlayer>> Vecteur de pointeurs uniques vers les ennemis créés.
 */
std::vector<std::unique_ptr<NonPlayer>> CharacterFactory::createNonPlayer(
    const sf::Vector2u &windowSize,
    const sf::Vector2f &offset)
{
    std::vector<std::unique_ptr<NonPlayer>> npcs;
    auto pnjTexture = std::make_shared<sf::Texture>();
    if (!pnjTexture->loadFromFile("../src/assets/images/nonPlayerBaseAnimation.png"))
    {
        std::cerr << "Failed to load nonPlayer texture! Using empty fallback texture." << std::endl;
        pnjTexture->create(32, 32);
    }

    // Candle 1
    auto candle1 = std::make_unique<Candle>("Candle1", 50, 10, 150.f, pnjTexture);
    candle1->setPosition(windowSize.x / 3.f, windowSize.y / 2.f);
    candle1->setAnimationParams(4, 32, 32, 6.f);
    candle1->setHitbox(0.f, 0.f, 32.f, 32.f);
    npcs.push_back(std::move(candle1));

    // Candle 2
    auto candle2 = std::make_unique<Candle>("Candle2", 60, 15, 150.f, pnjTexture);
    candle2->setPosition(windowSize.x / 2.f, windowSize.y / 1.8f);
    candle2->setAnimationParams(4, 32, 32, 6.f);
    candle2->setHitbox(0.f, 0.f, 32.f, 32.f);
    npcs.push_back(std::move(candle2));

    return npcs;
}
