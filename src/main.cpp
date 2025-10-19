#include <SFML/Graphics.hpp>
#include <iostream>
#include "factories/CharacterFactory.hpp"
#include "factories/GroundFactory.hpp"
#include "events/EventManager.hpp"
#include "DevMode.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //---------------------------------
    // Création des entités via les factories
    //---------------------------------
    auto player = CharacterFactory::createPlayer(window.getSize());
    auto pnj1 = CharacterFactory::createNonPlayer(window.getSize(), {2.5f, 2.f});
    auto grounds = GroundFactory::createDefaultGrounds(window.getSize());

    //---------------------------------
    // Liste globale des personnages
    //---------------------------------
    std::vector<GameCharacter *> allCharacters;
    allCharacters.push_back(player.get());
    allCharacters.push_back(pnj1.get());

    EventManager eventManager(window);
    DevMode dev(true);
    sf::Clock clock;

    bool isPaused = false;
    //---------------------------------
    // Boucle principale
    //---------------------------------

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements SFML pour la pause
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // --- Mettre en pause quand la fenêtre perd le focus ---
            if (event.type == sf::Event::LostFocus)
                isPaused = true;

            // --- Reprendre quand la fenêtre reprend le focus ---
            if (event.type == sf::Event::GainedFocus)
                isPaused = false;
        }

        // Si le jeu est en pause, on saute la mise à jour mais on garde l’affichage
        if (!isPaused)
        {
            eventManager.processEvents(*player, allCharacters);

            // Update
            for (auto *character : allCharacters)
            {
                if (character->isAlive())
                    character->update(deltaTime, grounds);
            }

            // Supprime les PNJ morts
            allCharacters.erase(
                std::remove_if(allCharacters.begin(), allCharacters.end(),
                               [](GameCharacter *c)
                               { return !c->isAlive() && dynamic_cast<NonPlayer *>(c); }),
                allCharacters.end());
        }

        // --- Rendu ---
        window.clear();

        for (auto *character : allCharacters)
            if (character->isAlive())
                character->draw(window);

        for (auto &g : grounds)
            g.draw(window);

        dev.drawInfo(window, *player, allCharacters);

        // Affiche un message de pause
        if (isPaused)
        {
            sf::Font font;
            if (font.loadFromFile("../src/assets/fonts/arial.ttf"))
            {
                sf::Text pauseText("Jeu en pause", font, 80);
                pauseText.setFillColor(sf::Color::White);
                pauseText.setOutlineColor(sf::Color::Black);
                pauseText.setOutlineThickness(3);
                pauseText.setPosition(window.getSize().x / 2.f - pauseText.getGlobalBounds().width / 2.f,
                                      window.getSize().y / 2.f - pauseText.getGlobalBounds().height / 2.f);
                window.draw(pauseText);
            }
        }

        window.display();
    }

    return 0;
}
