#include "HealthPotion.hpp"
#include "../characters/GameCharacter.hpp"

void HealthPotion::applyEffect(GameCharacter& character) {
    character.heal(healAmount); // Supposé que GameCharacter a une méthode heal(int)
}
