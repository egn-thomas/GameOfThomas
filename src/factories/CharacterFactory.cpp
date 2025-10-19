#include "CharacterFactory.hpp"
#include <iostream>

std::unique_ptr<Player> CharacterFactory::createPlayer(const sf::Vector2u &windowSize)
{
    sf::Texture *playerTexture = new sf::Texture();
    if (!playerTexture->loadFromFile("../src/assets/images/player.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
        return nullptr;
    }

    auto player = std::make_unique<Player>("Thomas", 100, 50, 450.f, *playerTexture);
    player->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    player->getSprite().setScale(0.1f, 0.1f);
    return player;
}

std::unique_ptr<NonPlayer> CharacterFactory::createNonPlayer(const sf::Vector2u &windowSize, const sf::Vector2f &offset)
{
    sf::Texture *pnjTexture = new sf::Texture();
    if (!pnjTexture->loadFromFile("../src/assets/images/nonPlayer.png"))
    {
        std::cerr << "Failed to load nonPlayer texture!" << std::endl;
        return nullptr;
    }

    auto pnj = std::make_unique<NonPlayer>("Enemy", 50, 10, 450.f, *pnjTexture);
    pnj->setPosition(windowSize.x / offset.x, windowSize.y / offset.y);
    pnj->getSprite().setScale(0.1f, 0.1f);
    return pnj;
}