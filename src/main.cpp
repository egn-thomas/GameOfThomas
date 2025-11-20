#ifndef GAME_OF_THOMAS_MAIN_HPP
#define GAME_OF_THOMAS_MAIN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "factories/CharacterFactory.hpp"
#include "events/EventManager.hpp"
#include "DevMode.hpp"

#include "./blocks/Block.hpp"
#include "./factories/BlockFactory.hpp"
#include "./blocks/BlockTypeUtils.hpp"
#include "./environnement/ModelGenerator.hpp"
#include "./environnement/Node.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //---------------------------------
    // Création du background de la fenêtre
    //---------------------------------

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../src/assets/images/castleBackground.png"))
    {
        std::cerr << "Erreur lors du chargement de la texture de fond." << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = float(windowSize.x) / textureSize.x;
    float scaleY = float(windowSize.y) / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);

    //---------------------------------
    // Création des entités via les factories
    //---------------------------------
    auto player = CharacterFactory::createPlayer(window.getSize());
    auto npcs = CharacterFactory::createNonPlayer(window.getSize(), {2.5f, 2.f});

    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Ground>> grounds;

    ModelGenerator mg(7, 4);
    for (Node *n : mg.getGrid())
    {
        BlockType t = getBlockTypeFromNode(n);

        std::cout << "Node pos=(" << n->getxPos() << "," << n->getyPos() << ") walls:[top=" << n->top << ",bot=" << n->bottom << ",left=" << n->left << ",right=" << n->right << "] -> BlockType=" << static_cast<int>(t) << std::endl;

        std::unique_ptr<Block> tile = BlockFactory::createBlocks(t, n->getxPos(), n->getyPos(), window);

        blocks.push_back(std::move(tile));

        auto created = blocks.back()->createGrounds();
        std::cout << "  -> Block created " << created.size() << " grounds" << std::endl;
        for (auto &g : created)
            grounds.push_back(std::move(g));
    }

    int mismatches = 0;
    const auto &gridRef = mg.getGrid();
    for (Node *n : gridRef)
    {
        int x = n->getxPos();
        int y = n->getyPos();

        if (x + 1 < mg.getWidth())
        {
            Node *r = gridRef[y * mg.getWidth() + (x + 1)];
            if (n->right != r->left)
            {
                std::cerr << "Symmetry mismatch: (" << x << "," << y << ") right=" << n->right
                          << " vs (" << r->getxPos() << "," << r->getyPos() << ") left=" << r->left << std::endl;
                ++mismatches;
            }
        }

        if (y + 1 < mg.getHeight())
        {
            Node *b = gridRef[(y + 1) * mg.getWidth() + x];
            if (n->bottom != b->top)
            {
                std::cerr << "Symmetry mismatch: (" << x << "," << y << ") bottom=" << n->bottom
                          << " vs (" << b->getxPos() << "," << b->getyPos() << ") top=" << b->top << std::endl;
                ++mismatches;
            }
        }
    }

    if (mismatches == 0)
        std::cout << "Node symmetry check: OK (no mismatches)\n";
    else
        std::cout << "Node symmetry check: " << mismatches << " mismatches found\n";

    {
        float tileSizeX = static_cast<float>(window.getSize().x) / static_cast<float>(mg.getWidth());
        float tileSizeY = static_cast<float>(window.getSize().y) / static_cast<float>(mg.getHeight());
        const float probeThickness = 6.f;

        auto isBlockedBetween = [&](float px, float py, float w, float h)
        {
            sf::FloatRect probe(px, py, w, h);
            for (const auto &g : grounds)
            {
                if (g->getBounds().intersects(probe))
                    return true;
            }
            return false;
        };

        int blockedCount = 0;
        for (Node *n : mg.getGrid())
        {
            int x = n->getxPos();
            int y = n->getyPos();

            if (x + 1 < mg.getWidth())
            {
                Node *r = mg.getGrid()[y * mg.getWidth() + (x + 1)];
                if (!n->right && !r->left)
                {
                    float edgeX = (x + 1) * tileSizeX;
                    float edgeY = y * tileSizeY + tileSizeY * 0.1f;
                    float edgeH = tileSizeY * 0.8f;
                    if (isBlockedBetween(edgeX - probeThickness / 2.f, edgeY, probeThickness, edgeH))
                    {
                        std::cerr << "Blocked opening between (" << x << "," << y << ") and (" << r->getxPos() << "," << r->getyPos() << ") at X=" << edgeX << "\n";
                        ++blockedCount;
                    }
                }
            }

            if (y + 1 < mg.getHeight())
            {
                Node *b = mg.getGrid()[(y + 1) * mg.getWidth() + x];
                if (!n->bottom && !b->top)
                {
                    float edgeX = x * tileSizeX + tileSizeX * 0.1f;
                    float edgeY = (y + 1) * tileSizeY;
                    float edgeW = tileSizeX * 0.8f;
                    if (isBlockedBetween(edgeX, edgeY - probeThickness / 2.f, edgeW, probeThickness))
                    {
                        std::cerr << "Blocked opening between (" << x << "," << y << ") and (" << b->getxPos() << "," << b->getyPos() << ") at Y=" << edgeY << "\n";
                        ++blockedCount;
                    }
                }
            }
        }

        if (blockedCount == 0)
            std::cout << "Geometry check: no blocked openings detected\n";
        else
            std::cout << "Geometry check: " << blockedCount << " blocked openings detected (see stderr)\n";
    }

    //---------------------------------
    // Liste globale des personnages
    //---------------------------------
    std::vector<GameCharacter *> allCharacters;
    allCharacters.push_back(player.get());
    for (auto &npc : npcs)
        allCharacters.push_back(npc.get());

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

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::LostFocus)
                isPaused = true;

            if (event.type == sf::Event::GainedFocus)
                isPaused = false;
        }

        if (!isPaused)
        {
            eventManager.processEvents(*player, allCharacters);

            // Update
            for (auto *character : allCharacters)
            {
                if (character->isAlive())
                    character->update(deltaTime, grounds);
            }

            allCharacters.erase(
                std::remove_if(allCharacters.begin(), allCharacters.end(),
                               [](GameCharacter *c)
                               { return !c->isAlive() && dynamic_cast<NonPlayer *>(c); }),
                allCharacters.end());
        }

        window.clear();

        window.draw(backgroundSprite);

        for (auto *character : allCharacters)
            if (character->isAlive())
                character->draw(window);

        for (auto &g : grounds)
            g->draw(window);

        dev.drawInfo(window, *player, allCharacters);

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

// #include <SFML/Graphics.hpp>
// #include "./environnement/ModelGenerator.hpp"
// #include "./environnement/Node.hpp"
// #include <vector>
// #include <iostream>

// void drawMazeSFML(const std::vector<Node *> &grid, int width, int height, int cellSize)
// {
//     sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Maze Debug");

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//             if (event.type == sf::Event::Closed)
//                 window.close();

//         window.clear(sf::Color::Black);

//         sf::RectangleShape line;
//         line.setFillColor(sf::Color::White);

//         for (Node *n : grid)
//         {
//             int px = n->getxPos() * cellSize;
//             int py = n->getyPos() * cellSize;

//             if (n->top)
//             {
//                 line.setSize(sf::Vector2f(cellSize, 2));
//                 line.setPosition(px, py);
//                 window.draw(line);
//             }
//             if (n->bottom)
//             {
//                 line.setSize(sf::Vector2f(cellSize, 2));
//                 line.setPosition(px, py + cellSize);
//                 window.draw(line);
//             }
//             if (n->left)
//             {
//                 line.setSize(sf::Vector2f(2, cellSize));
//                 line.setPosition(px, py);
//                 window.draw(line);
//             }
//             if (n->right)
//             {
//                 line.setSize(sf::Vector2f(2, cellSize));
//                 line.setPosition(px + cellSize, py);
//                 window.draw(line);
//             }
//         }

//         window.display();
//     }
// }

// int main()
// {
//     int width = 7;
//     int height = 4;
//     int cellSize = 50;

//     // Génération du labyrinthe
//     ModelGenerator mg(width, height);

//     // Debug : taille de la grille
//     std::cout << "Width=" << mg.getWidth()
//               << ", Height=" << mg.getHeight()
//               << ", Grid size=" << mg.getGrid().size() << std::endl;

//     // Debug : contenu de chaque node
//     for (Node *n : mg.getGrid())
//     {
//         std::cout << "Node x=" << n->getxPos()
//                   << " y=" << n->getyPos()
//                   << " top=" << n->top
//                   << " bottom=" << n->bottom
//                   << " left=" << n->left
//                   << " right=" << n->right
//                   << std::endl;
//     }

//     // Debug rapide : si la grille semble vide, enlever quelques murs
//     if (mg.getGrid().size() == 0)
//     {
//         std::cerr << "Warning : Grid is empty, forcing some walls for debug." << std::endl;
//         for (Node *n : mg.getGrid())
//         {
//             n->top = false;
//             n->left = false;
//         }
//     }

//     drawMazeSFML(mg.getGrid(), width, height, cellSize);
// }

#endif // GAME_OF_THOMAS_MAIN_HPP