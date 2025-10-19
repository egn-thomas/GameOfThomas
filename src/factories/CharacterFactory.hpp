#pragma once
#include <SFML/Graphics.hpp>
#include "../characters/Player.hpp"
#include "../characters/NonPlayer.hpp"
#include <vector>
#include <memory>

class CharacterFactory
{
public:
    static std::unique_ptr<Player> createPlayer(const sf::Vector2u &windowSize);
    static std::unique_ptr<NonPlayer> createNonPlayer(const sf::Vector2u &windowSize, const sf::Vector2f &offset);
};