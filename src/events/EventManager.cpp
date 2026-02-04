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
#include "../ui/UIManager.hpp"

void EventManager::processEvents(Player &player, std::vector<GameCharacter *> allCharacters, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    // Gestion de l'ouverture/fermeture de l'inventaire
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        if (!iKeyPressed) {
            inventoryOpen = !inventoryOpen;
            iKeyPressed = true;
            if (inventoryOpen) paused = true;
            else paused = false;
        }
    } else {
        iKeyPressed = false;
    }

    if (inventoryOpen) {
        // Navigation dans l'inventaire via UIManager
        if (uiManager) {
            InventoryMenu& invMenu = uiManager->getInventoryMenu();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (!leftPressed) { invMenu.moveSelection(-1); leftPressed = true; }
            } else leftPressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (!rightPressed) { invMenu.moveSelection(1); rightPressed = true; }
            } else rightPressed = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (!enterPressed) {
                    player.useItem(invMenu.getSelectedSlot());
                    enterPressed = true;
                }
            } else enterPressed = false;
        }
        // When inventory is open, skip other game inputs
        return;
    }

    // Ne traiter les événements du jeu que si pas en pause
    if (!paused)
    {
        handleKeyboard(player, 1.f / 60.f, allCharacters, grounds);
    }

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
void EventManager::handleKeyboard(Player &player, float deltaTime, std::vector<GameCharacter *> allCharacters, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    sf::Vector2f direction(0.f, 0.f);
    static sf::Vector2f lastGroundDirection(0.f, 0.f);  // Tracker last direction when grounded

    // Réinitialiser le flag d'interaction
    interactPressed = false;

    // Déplacemnts Standard (seulement au sol - en l'air, garde son élan)
    if (player.isOnGround() || player.isOnLadder())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            direction.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            direction.x += 1.f;
        lastGroundDirection = direction;  // Save the direction when grounded
    }
    else
    {
        // En l'air: réappliquer la dernière direction au sol (force le mouvement)
        direction = lastGroundDirection;
    }
    // Jump when on ground
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.isOnGround() && !player.isOnLadder())
    {
        player.jump();
    }

    // Ladder climbing: if player overlaps a ladder, allow vertical movement using Z (up) / S (down)
    bool verticalInput = false;
    if (player.isOnLadder())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            direction.y -= 1.f;
            verticalInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            direction.y += 1.f;
            verticalInput = true;
        }
        // When vertical input on a ladder, enable climbing mode on the player so gravity is disabled
        player.setClimbing(verticalInput);
    }
    else
    {
        // Ensure climbing mode is disabled when not on ladder
        player.setClimbing(false);
    }
    
    // Attaque a droite ou a gauche
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.attack(Direction::Right, allCharacters, grounds);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.attack(Direction::Left, allCharacters, grounds);
    }

    // Interaction (E pour ouvrir coffres, etc.)
    static bool eKeyPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        if (!eKeyPressed)
        {
            interactPressed = true;
            eKeyPressed = true;
        }
    }
    else
    {
        eKeyPressed = false;
    }

    // Vue de la carte (M pour map)
    static bool mKeyPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
    {
        if (!mKeyPressed)
        {
            toggleMapView();
            mKeyPressed = true;
        }
    }
    else
    {
        mKeyPressed = false;
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
