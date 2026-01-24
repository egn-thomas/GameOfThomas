#include "Candle.hpp"
#include "Player.hpp"
#include <cmath>
#include <iostream>

/**
 * @brief Constructeur de Candle
 */
Candle::Candle(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture)
    : NonPlayer(name, hp, mana, speed, texture)
{
}

/**
 * @brief Calcule la direction normalisée vers le joueur
 */
sf::Vector2f Candle::calculateDirectionToPlayer(const sf::Vector2f& playerPos)
{
    sf::Vector2f diff = playerPos - this->getPosition();
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    if (distance > 0.f)
    {
        return sf::Vector2f(diff.x / distance, diff.y / distance);
    }
    return sf::Vector2f(0.f, 0.f);
}

/**
 * @brief Met à jour le comportement de la chandelle
 * Poursuite du joueur si celui-ci est dans la zone de détection
 */
void Candle::updateBehavior(float deltaTime, Player* player, const std::vector<std::unique_ptr<Ground>>& grounds)
{
    if (player == nullptr || !player->isAlive())
        return;
    
    sf::Vector2f myPos = this->getPosition();
    sf::Vector2f playerPos = player->getPosition();
    
    // Calculer la distance au joueur
    sf::Vector2f diff = playerPos - myPos;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // Si le joueur est en portée de détection
    if (distance <= detectionRange)
    {
        // Calculer la direction vers le joueur
        sf::Vector2f direction = calculateDirectionToPlayer(playerPos);
        
        // Appliquer la vélocité horizontale pour suivre le joueur
        sf::Vector2f velocity = this->getVelocity();
        velocity.x = direction.x * this->getSpeed();
        
        // Met à jour la position en appliquant les forces et collisions
        this->move(sf::Vector2f(velocity.x * deltaTime, 0.f));
        
        // Appliquer la gravité et les collisions
        this->update(deltaTime, grounds);
        
        // Vérifier si le joueur est assez proche pour attaquer
        if (distance <= attackRange)
        {
            // Logique d'attaque à implémenter plus tard
            // std::cout << "Candle en portée d'attaque!" << std::endl;
        }
    }
    else
    {
        // Comportement inactif si joueur non détecté
        sf::Vector2f velocity = this->getVelocity();
        velocity.x = 0.f;
        this->update(deltaTime, grounds);
    }
}
