#include "EventManager.hpp"
#include "Event.hpp"
#include "MoveEvent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <iostream>



EventManager::EventManager(sf::RenderWindow& win)
    : window(win) {}

void EventManager::processEvents(Player& player) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    handleKeyboard(player, 1.f / 60.f);
     
    // Exécute tous les événements
    for (auto& ev : eventQueue) {
        ev->execute();
    }

    eventQueue.clear();
}

void EventManager::handleKeyboard(Player& player, float deltaTime) {
    sf::Vector2f direction(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))  direction.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.isOnGround()) {
        player.jump(700);
    }

    if (direction.x != 0.f || direction.y != 0.f) {
        // Normaliser le vecteur si nécessaire
        float length = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        if (length > 0.f) direction /= length;

        eventQueue.push_back(std::make_unique<MoveEvent>(
            player,
            direction * player.getSpeed() * deltaTime
        ));
    }
}
