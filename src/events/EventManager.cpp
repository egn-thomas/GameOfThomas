#include "EventManager.hpp"
#include "Event.hpp"
#include "MoveEvent.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
#include <iostream>

/**
 * @brief L'EventManager s'occupe de créer tous les évenements et et tous les executer à chaque frame.
 */
EventManager::EventManager(sf::RenderWindow &win)
    : window(win) {}

/**
 * @brief Traite les événements de la fenêtre et exécute les événements en attente.
 *
 * @param player Le joueur principal.
 * @param allCharacters Tous les personnages du jeu (joueur et PNJ).
 */
void EventManager::processEvents(Player &player, std::vector<GameCharacter *> allCharacters)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    handleKeyboard(player, 1.f / 60.f, allCharacters);

    // Exécute tous les événements
    for (auto &ev : eventQueue)
    {
        ev->execute();
    }

    eventQueue.clear();
}

/**
 * @brief Gère les entrées clavier pour le joueur et crée les événements correspondants.
 *
 * @param player Le joueur principal.
 * @param deltaTime Le temps écoulé depuis la dernière frame.
 * @param allCharacters Tous les personnages du jeu (joueur et PNJ).
 */
void EventManager::handleKeyboard(Player &player, float deltaTime, std::vector<GameCharacter *> allCharacters)
{
    sf::Vector2f direction(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        direction.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        direction.x += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.isOnGround())
    {
        player.jump(700);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.attack(Direction::Right, allCharacters);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.attack(Direction::Left, allCharacters);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.isCanDash())
    {
        int dir = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            dir = 1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            dir = -1;

        if (dir != 0)
            player.startDash(dir);
    }

    if (direction.x != 0.f || direction.y != 0.f)
    {
        // Normaliser le vecteur si nécessaire
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.f)
            direction /= length;

        eventQueue.push_back(std::make_unique<MoveEvent>(
            player,
            direction * player.getSpeed() * deltaTime));
    }
}
