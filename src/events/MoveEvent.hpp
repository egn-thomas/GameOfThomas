#ifndef MOVEEVENT_HPP
#define MOVEEVENT_HPP

#include "Event.hpp"
#include "../characters/GameCharacter.hpp"
#include <SFML/System/Vector2.hpp>

class MoveEvent : public Event {
private:
    GameCharacter& character;
    sf::Vector2f direction;

public:
    MoveEvent(GameCharacter& c, const sf::Vector2f& dir);
    void execute() override;
};

#endif