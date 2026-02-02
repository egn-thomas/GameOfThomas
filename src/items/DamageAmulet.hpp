#pragma once

#include "Item.hpp"
#include "../characters/GameCharacter.hpp"

class DamageAmulet : public Item {
    int bonus;
public:
    explicit DamageAmulet(int b) : bonus(b) {}
    void applyEffect(GameCharacter& character) override;
    std::string getName() const override { return "Amulette de dégâts"; }
    std::string getDescription() const override { return "Augmente les dégâts de " + std::to_string(bonus) + " points."; }
    std::string getSpritePath() const override { return "assets/images/items/damagesAmulet.png"; }
};