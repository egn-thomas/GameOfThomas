#pragma once
#include "GameCharacter.hpp"
#include <memory>
#include <vector>

class Ground;
class Player;

class NonPlayer : public GameCharacter {
private:
public:
    // constructeur
    NonPlayer(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);

    // dessin éventuellement surchargé (si spécifique)
    void draw(sf::RenderWindow& window) override;
    
    /**
     * @brief Méthode virtuelle pour le comportement IA
     * À surcharger dans les classes dérivées
     */
    virtual void updateBehavior(float deltaTime, Player* player, const std::vector<std::unique_ptr<Ground>>& grounds);
};
