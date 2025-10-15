#include "MoveEvent.hpp"

MoveEvent::MoveEvent(GameCharacter& c, const sf::Vector2f& dir)
    : character(c), direction(dir) {}

void MoveEvent::execute() {
    character.move(direction);
}