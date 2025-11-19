// #include <SFML/Graphics.hpp>
// #include <iostream>
// #include "factories/CharacterFactory.hpp"
// #include "factories/GroundFactory.hpp"
// #include "events/EventManager.hpp"
// #include "DevMode.hpp"

// int main()
// {
//     sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
//     window.setFramerateLimit(60);
//     window.setVerticalSyncEnabled(true);

//     //---------------------------------
//     // Création des entités via les factories
//     //---------------------------------
//     auto player = CharacterFactory::createPlayer(window.getSize());
//     auto npcs = CharacterFactory::createNonPlayer(window.getSize(), {2.5f, 2.f});
//     auto grounds = GroundFactory::createDefaultGrounds(window.getSize());

//     //---------------------------------
//     // Liste globale des personnages
//     //---------------------------------
//     std::vector<GameCharacter *> allCharacters;
//     allCharacters.push_back(player.get());
//     for (auto &npc : npcs)
//         allCharacters.push_back(npc.get());

//     EventManager eventManager(window);
//     DevMode dev(true);
//     sf::Clock clock;

//     bool isPaused = false;

//     //---------------------------------
//     // Boucle principale
//     //---------------------------------
//     while (window.isOpen())
//     {
//         float deltaTime = clock.restart().asSeconds();

//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             if (event.type == sf::Event::LostFocus)
//                 isPaused = true;

//             if (event.type == sf::Event::GainedFocus)
//                 isPaused = false;
//         }

//         if (!isPaused)
//         {
//             eventManager.processEvents(*player, allCharacters);

//             // Update
//             for (auto *character : allCharacters)
//             {
//                 if (character->isAlive())
//                     character->update(deltaTime, grounds);
//             }

//             allCharacters.erase(
//                 std::remove_if(allCharacters.begin(), allCharacters.end(),
//                                [](GameCharacter *c)
//                                { return !c->isAlive() && dynamic_cast<NonPlayer *>(c); }),
//                 allCharacters.end());
//         }

//         window.clear();

//         for (auto *character : allCharacters)
//             if (character->isAlive())
//                 character->draw(window);

//         for (auto &g : grounds)
//             g->draw(window);

//         dev.drawInfo(window, *player, allCharacters);

//         if (isPaused)
//         {
//             sf::Font font;
//             if (font.loadFromFile("../src/assets/fonts/arial.ttf"))
//             {
//                 sf::Text pauseText("Jeu en pause", font, 80);
//                 pauseText.setFillColor(sf::Color::White);
//                 pauseText.setOutlineColor(sf::Color::Black);
//                 pauseText.setOutlineThickness(3);
//                 pauseText.setPosition(window.getSize().x / 2.f - pauseText.getGlobalBounds().width / 2.f,
//                                       window.getSize().y / 2.f - pauseText.getGlobalBounds().height / 2.f);
//                 window.draw(pauseText);
//             }
//         }

//         window.display();
//     }

//     return 0;
// }

#include <SFML/Graphics.hpp>
#include "./environnement/ModelGenerator.hpp"
#include "./environnement/Node.hpp"
#include <vector>
#include <iostream>

void drawMazeSFML(const std::vector<Node *> &grid, int width, int height, int cellSize)
{
    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Maze Debug");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::Black);

        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);

        for (Node *n : grid)
        {
            int px = n->getxPos() * cellSize;
            int py = n->getyPos() * cellSize;

            if (n->top)
            {
                line.setSize(sf::Vector2f(cellSize, 2));
                line.setPosition(px, py);
                window.draw(line);
            }
            if (n->bottom)
            {
                line.setSize(sf::Vector2f(cellSize, 2));
                line.setPosition(px, py + cellSize);
                window.draw(line);
            }
            if (n->left)
            {
                line.setSize(sf::Vector2f(2, cellSize));
                line.setPosition(px, py);
                window.draw(line);
            }
            if (n->right)
            {
                line.setSize(sf::Vector2f(2, cellSize));
                line.setPosition(px + cellSize, py);
                window.draw(line);
            }
        }

        window.display();
    }
}

int main()
{
    int width = 7;
    int height = 4;
    int cellSize = 50;

    // Génération du labyrinthe
    ModelGenerator mg(width, height);

    // Debug : taille de la grille
    std::cout << "Width=" << mg.getWidth()
              << ", Height=" << mg.getHeight()
              << ", Grid size=" << mg.getGrid().size() << std::endl;

    // Debug : contenu de chaque node
    for (Node *n : mg.getGrid())
    {
        std::cout << "Node x=" << n->getxPos()
                  << " y=" << n->getyPos()
                  << " top=" << n->top
                  << " bottom=" << n->bottom
                  << " left=" << n->left
                  << " right=" << n->right
                  << std::endl;
    }

    // Debug rapide : si la grille semble vide, enlever quelques murs
    if (mg.getGrid().size() == 0)
    {
        std::cerr << "Warning : Grid is empty, forcing some walls for debug." << std::endl;
        for (Node *n : mg.getGrid())
        {
            n->top = false;
            n->left = false;
        }
    }

    drawMazeSFML(mg.getGrid(), width, height, cellSize);
}
