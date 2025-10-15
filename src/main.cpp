#include <SFML/Graphics.hpp>
#include <iostream>
#include "characters/Player.hpp"
#include "events/EventManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../src/assets/images/player.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
        return -1;
    }

    Player player("Thomas", 100, 50, 5.f, playerTexture);
    player.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    player.getSprite().setScale(0.1f, 0.1f);
    EventManager eventManager(window);

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        eventManager.processEvents(player);

        player.update();

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}