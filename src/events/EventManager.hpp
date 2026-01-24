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
    bool isMapViewActive() const { return showMapView; }
    void toggleMapView() { showMapView = !showMapView; }
    bool isPaused() const { return paused; }
    void setPaused(bool state) { paused = state; }

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Event>> eventQueue;
    bool showMapView = false;
    bool paused = false;
    
    void handleKeyboard(Player& player, float deltaTime,  std::vector<GameCharacter *> allCharacters);
};