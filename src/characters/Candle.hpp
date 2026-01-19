#pragma once
#include "NonPlayer.hpp"
#include <memory>

class Player; // Forward declaration

class Candle : public NonPlayer {
private:
    const float detectionRange = 500.f;  // Distance à partir de laquelle la chandelle détecte le joueur
    const float attackRange = 50.f;      // Distance minimale pour attaquer
    
    /**
     * @brief Calcule la direction vers le joueur
     * @param playerPos Position du joueur
     * @return Direction normalisée vers le joueur
     */
    sf::Vector2f calculateDirectionToPlayer(const sf::Vector2f& playerPos);
    
public:
    /**
     * @brief Constructeur de Candle
     */
    Candle(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);
    
    /**
     * @brief Met à jour le comportement de la chandelle (poursuite du joueur)
     * @param deltaTime Temps écoulé depuis le dernier update
     * @param player Pointeur sur le joueur
     * @param grounds Vecteur des terrains pour les collisions
     */
    void updateBehavior(float deltaTime, Player* player, const std::vector<std::unique_ptr<Ground>>& grounds);
};
