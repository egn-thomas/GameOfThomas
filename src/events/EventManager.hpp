#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>
#include "Event.hpp"
#include "../characters/Player.hpp"


class GameCharacter;

class UIManager; // forward

class EventManager {
public:
    explicit EventManager(sf::RenderWindow& win);
    void processEvents(Player &player, std::vector<GameCharacter *> allCharacters);
    bool isMapViewActive() const { return showMapView; }
    void toggleMapView() { showMapView = !showMapView; }
    bool isPaused() const { return paused; }
    void setPaused(bool state) { paused = state; }
    bool isInteractPressed() const { return interactPressed; }

    // Inventaire
    void setUIManager(UIManager* ui) { uiManager = ui; }
    bool isInventoryOpen() const { return inventoryOpen; }

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Event>> eventQueue;
    bool showMapView = false;
    bool paused = false;
    bool interactPressed = false;

    // Inventory state
    UIManager* uiManager = nullptr;
    bool inventoryOpen = false;
    bool iKeyPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    bool enterPressed = false;
    
    void handleKeyboard(Player& player, float deltaTime,  std::vector<GameCharacter *> allCharacters);
};