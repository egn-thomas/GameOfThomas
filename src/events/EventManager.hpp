#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "Event.hpp"

class GameCharacter;

class EventManager {
public:
    explicit EventManager(sf::RenderWindow& win);

    void addEvent(std::unique_ptr<Event> ev);
    void processEvents(GameCharacter& player);

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Event>> eventQueue;
};
