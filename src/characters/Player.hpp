#pragma once
#include "GameCharacter.hpp"
#include <memory>

class Player : public GameCharacter {
protected:
    const float jumpStrength = 500.f;
public:
    // constructeur
    Player(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);

    void jump();

    // dessin éventuellement surchargé (si spécifique)
    void draw(sf::RenderWindow& window) override;
};
