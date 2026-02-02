#pragma once
#include <SFML/Graphics.hpp>
#include "../characters/Player.hpp"
#include "../characters/Candle.hpp"
#include "../items/HealthPotion.hpp"
#include "../items/ManaPotion.hpp"
#include "../items/HealthAmulet.hpp"
#include "../items/DamageAmulet.hpp"
#include <vector>
#include <memory>

class CharacterFactory
{
public:
    static std::unique_ptr<Player> createPlayer(const sf::Vector2u &windowSize);
    static std::vector<std::unique_ptr<NonPlayer>> createNonPlayer(const sf::Vector2u &windowSize, const sf::Vector2f &offset);
    
    /**
     * @brief Crée une Candle à une position spécifique
     * @param position Position du mob dans le monde
     * @return std::unique_ptr<Candle> Pointeur unique vers la Candle créée
     */
    static std::unique_ptr<Candle> createCandle(const sf::Vector2f &position);
};