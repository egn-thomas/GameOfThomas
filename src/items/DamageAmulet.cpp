#include "DamageAmulet.hpp"

void DamageAmulet::applyEffect(GameCharacter& character) {
    character.addDamageBonus(bonus);
}