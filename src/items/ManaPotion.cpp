#include "ManaPotion.hpp"

void ManaPotion::applyEffect(GameCharacter& character) {
    character.restoreMana(restoreAmount);
}