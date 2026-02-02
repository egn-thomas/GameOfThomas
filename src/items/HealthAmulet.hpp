#pragma once

#include "Item.hpp"
#include "../characters/GameCharacter.hpp"

class HealthAmulet : public Item {
    int bonus;
public:
    explicit HealthAmulet(int b) : bonus(b) {}
    void applyEffect(GameCharacter& character) override;
    std::string getName() const override { return "Amulette de santé"; }
    std::string getDescription() const override { return "Augmente la santé maximale de " + std::to_string(bonus) + " points."; }
    std::string getSpritePath() const override { return "assets/images/items/healthAmulet.png"; }
};