#include "EventManager.hpp"
#include "../characters/GameCharacter.hpp"
#include "Event.hpp"
#include <SFML/Window/Event.hpp>

EventManager::EventManager(sf::RenderWindow& win)
    : window(win) {}

void EventManager::addEvent(std::unique_ptr<Event> ev) {
    eventQueue.push_back(std::move(ev));
}

void EventManager::processEvents(GameCharacter& player) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    for (auto& ev : eventQueue) {
        ev->execute();
    }

    eventQueue.clear();
}