#pragma once
#include "GameCharacter.hpp"
#include <memory>

class Player : public GameCharacter {
private:
public:
    // constructeur
    Player(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture);

    // override de la méthode virtuelle
    void update() override;

    // dessin éventuellement surchargé (si spécifique)
    void draw(sf::RenderWindow& window) override;
};
