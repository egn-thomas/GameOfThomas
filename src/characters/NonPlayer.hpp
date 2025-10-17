#pragma once
#include "GameCharacter.hpp"
#include <memory>

class NonPlayer : public GameCharacter {
private:
public:
    // constructeur
    NonPlayer(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture);

    // dessin éventuellement surchargé (si spécifique)
    void draw(sf::RenderWindow& window) override;
};
