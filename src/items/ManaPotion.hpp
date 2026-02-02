#pragma once

#include "Item.hpp"
#include "../characters/GameCharacter.hpp"

class ManaPotion : public Item {
    int restoreAmount;
public:
    explicit ManaPotion(int amt) : restoreAmount(amt) {}
    void applyEffect(GameCharacter& character) override;
    std::string getName() const override { return "Potion de mana"; }
    std::string getDescription() const override { return "Restaure " + std::to_string(restoreAmount) + " points de mana."; }
    std::string getSpritePath() const override { return "assets/images/items/manaPotion.png"; }
};