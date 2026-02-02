#include "HealthAmulet.hpp"

void HealthAmulet::applyEffect(GameCharacter& character) {
    character.increaseMaxHp(bonus);
}