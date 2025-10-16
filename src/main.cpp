#include <SFML/Graphics.hpp>
#include <iostream>
#include "characters/Player.hpp"
#include "events/EventManager.hpp"
#include "environnement/Ground.hpp"
#include "DevMode.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //---------------------------------
    // Player
    //---------------------------------
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../src/assets/images/player.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
        return -1;
    }
    Player player("Thomas", 100, 50, 450.f, playerTexture);
    player.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    player.getSprite().setScale(0.1f, 0.1f);

    //---------------------------------
    // Ground
    //---------------------------------
    Ground ground(0.f, window.getSize().y - 50.f, window.getSize().x, 50.f);
    Ground plateforme(100.f, 900.f, 500.f, 10.f);
    Ground plateforme1(600.f, 750.f, 500.f, 10.f);
    Ground plateforme2(100.f, 600.f, 500.f, 10.f);
    Ground plateforme3(600.f, 450.f, 500.f, 10.f);
    Ground plateforme4(100.f, 300.f, 500.f, 10.f);
    Ground plateformeVerticale(1300.f, 900.f, 100.f, 250.f);

    std::vector<Ground> grounds;
    grounds.emplace_back(ground);
    grounds.emplace_back(plateforme);
    grounds.emplace_back(plateforme1);
    grounds.emplace_back(plateforme2);
    grounds.emplace_back(plateforme3);
    grounds.emplace_back(plateformeVerticale);

    EventManager eventManager(window);

    DevMode dev(true);
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        eventManager.processEvents(player);

        player.update(deltaTime, grounds);

        window.clear();
        player.draw(window);
        ground.draw(window);
        plateforme.draw(window);
        plateforme1.draw(window);
        plateforme2.draw(window);
        plateforme3.draw(window);
        plateformeVerticale.draw(window);

        dev.drawInfo(window, player);
        
        window.display();
    }

    return 0;
}