#include "Candle.hpp"
#include "Player.hpp"
#include <cmath>
#include <iostream>

/**
 * @brief Constructeur de Candle
 */
Candle::Candle(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture)
    : NonPlayer(name, hp, mana, stamina, speed, texture)
{
    // Réduire la taille de 20% par rapport au scale par défaut (4.0 -> 3.2)
    sprite.setScale(3.2f, 3.2f);
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
    
    // Décrémenter le cooldown d'attaque
    if (attackCooldown > 0.f)
    {
        attackCooldown -= deltaTime;
    }
    
    sf::Vector2f myPos = this->getPosition();
    sf::Vector2f playerPos = player->getPosition();
    
    // Calculer la distance au joueur
    sf::Vector2f diff = playerPos - myPos;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    // Si le joueur est en portée de détection ET en ligne de vue
    if (distance <= detectionRange && GameCharacter::hasLineOfSight(myPos, playerPos, grounds))
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
        
        // Vérifier si le joueur est en portée d'attaque et si le cooldown est écoulé
        if (distance <= attackRange && attackCooldown <= 0.f)
        {
            // Déterminer la direction d'attaque (gauche ou droite)
            Direction attackDirection = (direction.x < 0.f) ? Direction::Left : Direction::Right;
            
            // Effectuer l'attaque avec les paramètres définis
            this->attack(attackDirection, {player}, AttackType::CandleAttack, grounds);
            
            // Réinitialiser le cooldown
            attackCooldown = attackCooldownDuration;
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
