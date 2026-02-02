#pragma once

#include "Item.hpp"
#include "../characters/GameCharacter.hpp"

class HealthPotion : public Item {
    int healAmount;
public:
    HealthPotion(int amount) : healAmount(amount) {}
    void applyEffect(GameCharacter& character) override;
    std::string getName() const override { return "Potion de soin"; }
    std::string getDescription() const override { return "Restaure " + std::to_string(healAmount) + " points de vie."; }
    std::string getSpritePath() const override { return "assets/images/items/healthPotion.png"; }
};
