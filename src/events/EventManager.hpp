#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "../characters/Player.hpp"


class GameCharacter;

class EventManager {
public:
    explicit EventManager(sf::RenderWindow& win);
    void processEvents(Player &player, std::vector<GameCharacter *> allCharacters);

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Event>> eventQueue;
    
    void handleKeyboard(Player& player, float deltaTime,  std::vector<GameCharacter *> allCharacters);
};