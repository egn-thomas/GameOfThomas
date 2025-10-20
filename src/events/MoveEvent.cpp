#include "MoveEvent.hpp"

/**
 * @brief Constructeur de MoveEvent.
 * @note Sert à demander à un GameCharacter de se déplacer dans une direction donnée.
 * @param c Référence au GameCharacter à déplacer.
 * @param dir Vecteur de direction du déplacement.
 */
MoveEvent::MoveEvent(GameCharacter& c, const sf::Vector2f& dir)
    : character(c), direction(dir) {}

void MoveEvent::execute() {
    character.move(direction);
}