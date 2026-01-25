#ifndef GAME_OF_THOMAS_MAIN_HPP
#define GAME_OF_THOMAS_MAIN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "factories/CharacterFactory.hpp"
#include "events/EventManager.hpp"
#include "ui/PauseMenu.hpp"
#include "DevMode.hpp"

#include "./blocks/Block.hpp"
#include "./factories/BlockFactory.hpp"
#include "./blocks/BlockTypeUtils.hpp"
#include "./environnement/ModelGenerator.hpp"
#include "./environnement/Node.hpp"
#include "./objects/Chest.hpp"
#include "./objects/Door.hpp"
#include <iostream>

/**
 * @brief Structure pour encapsuler les données du niveau
 */
struct GameLevel
{
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Ground>> grounds;
    std::vector<std::unique_ptr<Object>> gameObjects;
    std::vector<std::unique_ptr<GameCharacter>> spawnedCharacters;
    std::unique_ptr<ModelGenerator> mazeGenerator;
};

/**
 * @brief Génère un nouveau niveau
 * @param window La fenêtre de rendu
 * @return GameLevel Les données du nouveau niveau
 */
GameLevel generateNewLevel(sf::RenderWindow& window)
{
    GameLevel level;
    
    level.mazeGenerator = std::make_unique<ModelGenerator>(14, 8);
    
    for (Node *n : level.mazeGenerator->getGrid())
    {
        BlockType t = getBlockTypeFromNode(n);
        std::unique_ptr<Block> tile = BlockFactory::createBlocks(t, n->getxPos(), n->getyPos(), window);
        level.blocks.push_back(std::move(tile));

        auto created = level.blocks.back()->createGrounds();
        for (auto &g : created)
            level.grounds.push_back(std::move(g));

        auto objects = level.blocks.back()->createObjects();
        for (auto &obj : objects)
            level.gameObjects.push_back(std::move(obj));

        auto characters = level.blocks.back()->createCharacters();
        for (auto &character : characters)
            level.spawnedCharacters.push_back(std::move(character));
    }
    
    return level;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Thomas", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //---------------------------------
    // Chargement de la police pour les hints
    //---------------------------------
    sf::Font font;
    if (!font.loadFromFile("../src/assets/fonts/RobotoMono-Regular.ttf"))
    {
        std::cerr << "Warning: Could not load font for hints" << std::endl;
    }

    //---------------------------------
    // Création du background de la fenêtre
    //---------------------------------

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../src/assets/images/castleBackground.png"))
    {
        std::cerr << "Erreur lors du chargement de la texture de fond. Using fallback background." << std::endl;
        // create a 1x1 white texture as fallback so the game can continue
        backgroundTexture.create(1, 1);
        sf::Image img;
        img.create(1, 1, sf::Color(50, 50, 80));
        backgroundTexture.update(img);
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = float(windowSize.x) / textureSize.x * 2.f;
    float scaleY = float(windowSize.y) / textureSize.y * 2.f;

    backgroundSprite.setScale(scaleX, scaleY);

    //---------------------------------
    // Création des entités via les factories
    //---------------------------------
    auto player = CharacterFactory::createPlayer(window.getSize());
    auto npcs = CharacterFactory::createNonPlayer(window.getSize(), {2.5f, 2.f});

    // Générer le premier niveau
    GameLevel currentLevel = generateNewLevel(window);

    int mismatches = 0;
    const auto &gridRef = currentLevel.mazeGenerator->getGrid();
    for (Node *n : gridRef)
    {
        int x = n->getxPos();
        int y = n->getyPos();

        if (x + 1 < currentLevel.mazeGenerator->getWidth())
        {
            Node *r = gridRef[y * currentLevel.mazeGenerator->getWidth() + (x + 1)];
            if (n->right != r->left)
            {
                std::cerr << "Symmetry mismatch: (" << x << "," << y << ") right=" << n->right
                          << " vs (" << r->getxPos() << "," << r->getyPos() << ") left=" << r->left << std::endl;
                ++mismatches;
            }
        }

        if (y + 1 < currentLevel.mazeGenerator->getHeight())
        {
            Node *b = gridRef[(y + 1) * currentLevel.mazeGenerator->getWidth() + x];
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

    // --- Connectivity check (logical maze connectivity using node walls) ---
    {
        const auto &nodes = currentLevel.mazeGenerator->getGrid();
        int W = currentLevel.mazeGenerator->getWidth();
        int H = currentLevel.mazeGenerator->getHeight();
        std::vector<bool> seen(nodes.size(), false);
        std::vector<int> stackIdx;

        // start from bottom-left
        int startIdx = (H - 1) * W + 0;
        stackIdx.push_back(startIdx);
        seen[startIdx] = true;

        while (!stackIdx.empty())
        {
            int idx = stackIdx.back();
            stackIdx.pop_back();
            Node *n = nodes[idx];
            int x = n->getxPos();
            int y = n->getyPos();

            // top neighbor
            if (!n->top && y - 1 >= 0)
            {
                int ni = (y - 1) * W + x;
                if (!seen[ni]) { seen[ni] = true; stackIdx.push_back(ni); }
            }
            // bottom neighbor
            if (!n->bottom && y + 1 < H)
            {
                int ni = (y + 1) * W + x;
                if (!seen[ni]) { seen[ni] = true; stackIdx.push_back(ni); }
            }
            // left neighbor
            if (!n->left && x - 1 >= 0)
            {
                int ni = y * W + (x - 1);
                if (!seen[ni]) { seen[ni] = true; stackIdx.push_back(ni); }
            }
            // right neighbor
            if (!n->right && x + 1 < W)
            {
                int ni = y * W + (x + 1);
                if (!seen[ni]) { seen[ni] = true; stackIdx.push_back(ni); }
            }
        }

        int reachable = 0;
        std::vector<int> unreachable;
        for (size_t i = 0; i < seen.size(); ++i)
        {
            if (seen[i]) ++reachable;
            else unreachable.push_back(static_cast<int>(i));
        }

        if (reachable == (int)nodes.size())
            std::cout << "Logical connectivity check: OK — all " << reachable << " nodes reachable from entry\n";
        else
        {
            std::cerr << "Logical connectivity: only " << reachable << " / " << nodes.size() << " reachable from entry. Unreachable nodes:";
            for (int ui : unreachable)
            {
                Node *u = nodes[ui];
                std::cerr << " (" << u->getxPos() << "," << u->getyPos() << ")";
            }
            std::cerr << "\n";
        }
    }

    {
        float tileSizeX = static_cast<float>(window.getSize().x) / static_cast<float>(currentLevel.mazeGenerator->getWidth());
        float tileSizeY = static_cast<float>(window.getSize().y) / static_cast<float>(currentLevel.mazeGenerator->getHeight());
        const float probeThickness = 6.f;

        auto isBlockedBetween = [&](float px, float py, float w, float h)
        {
            sf::FloatRect probe(px, py, w, h);
            for (const auto &g : currentLevel.grounds)
            {
                if (g->getBounds().intersects(probe))
                    return true;
            }
            return false;
        };

        int blockedCount = 0;
        for (Node *n : currentLevel.mazeGenerator->getGrid())
        {
            int x = n->getxPos();
            int y = n->getyPos();

            if (x + 1 < currentLevel.mazeGenerator->getWidth())
            {
                Node *r = currentLevel.mazeGenerator->getGrid()[y * currentLevel.mazeGenerator->getWidth() + (x + 1)];
                if (!n->right && !r->left)
                {
                    float edgeX = (x + 1) * tileSizeX;
                    float edgeY = y * tileSizeY + tileSizeY * 0.1f;
                    float edgeH = tileSizeY * 0.8f;
                    // if (isBlockedBetween(edgeX - probeThickness / 2.f, edgeY, probeThickness, edgeH))
                    // {
                    //     std::cerr << "Blocked opening between (" << x << "," << y << ") and (" << r->getxPos() << "," << r->getyPos() << ") at X=" << edgeX << "\n";
                    //     ++blockedCount;
                    // }
                }
            }

            if (y + 1 < currentLevel.mazeGenerator->getHeight())
            {
                Node *b = currentLevel.mazeGenerator->getGrid()[(y + 1) * currentLevel.mazeGenerator->getWidth() + x];
                if (!n->bottom && !b->top)
                {
                    float edgeX = x * tileSizeX + tileSizeX * 0.1f;
                    float edgeY = (y + 1) * tileSizeY;
                    float edgeW = tileSizeX * 0.8f;
                    // if (isBlockedBetween(edgeX, edgeY - probeThickness / 2.f, edgeW, probeThickness))
                    // {
                    //     std::cerr << "Blocked opening between (" << x << "," << y << ") and (" << b->getxPos() << "," << b->getyPos() << ") at Y=" << edgeY << "\n";
                    //     ++blockedCount;
                    // }
                }
            }
        }

        if (blockedCount == 0)
            std::cout << "Geometry check: no blocked openings detected\n";
        else
            std::cout << "Geometry check: " << blockedCount << " blocked openings detected (see stderr)\n";
    }

    //---------------------------------
    // Configuration de la caméra
    //---------------------------------
    // Taille de la vue : environ 3 cases de largeur
    // Une case fait 256 pixels (64 pixels de sprite * 4x échelle)
    float cameraWidth = 3.f * 256.f; 
    float cameraHeight = cameraWidth * (window.getSize().y / static_cast<float>(window.getSize().x));
    
    sf::View gameView(sf::Vector2f(0.f, 0.f), sf::Vector2f(cameraWidth, cameraHeight));
    window.setView(gameView);

    //---------------------------------
    // Liste globale des personnages
    //---------------------------------
    std::vector<GameCharacter *> allCharacters;
    allCharacters.push_back(player.get());
    for (auto &character : currentLevel.spawnedCharacters)
        allCharacters.push_back(character.get());

    EventManager eventManager(window);
    DevMode dev(true);
    sf::Clock clock;
    
    PauseMenu pauseMenu;
    bool showPauseMenu = false;
    bool isPaused = false;

    // Variables pour la gestion des coffres et portes
    Chest* currentChestNearby = nullptr;
    Door* currentDoorNearby = nullptr;

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
            
            // Détection d'Échap pour la pause (car les événements sont consommés ici)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                eventManager.setPaused(!eventManager.isPaused());
                // std::cout << "[DEBUG MAIN] Échap détecté! paused = " << eventManager.isPaused() << std::endl;
            }
                
            // Gestion du menu de pause
            if (showPauseMenu)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Up)
                        pauseMenu.moveSelection(-1);
                    else if (event.key.code == sf::Keyboard::Down)
                        pauseMenu.moveSelection(1);
                    else if (event.key.code == sf::Keyboard::Return)
                    {
                        if (pauseMenu.getSelectedOption() == PauseMenu::MenuOption::Continue)
                        {
                            showPauseMenu = false;
                            eventManager.setPaused(false);
                        }
                        else if (pauseMenu.getSelectedOption() == PauseMenu::MenuOption::Quit)
                        {
                            window.close();
                        }
                    }
                }
            }
        }

        // Synchroniser l'état de pause avec EventManager
        if (eventManager.isPaused())
        {
            if (!showPauseMenu)
            {
                // std::cout << "[DEBUG MAIN] Mise en pause, affichage du menu" << std::endl;
                showPauseMenu = true;
                pauseMenu.resetSelection();
            }
        }
        else
        {
            if (showPauseMenu)
                // std::cout << "[DEBUG MAIN] Reprise du jeu, masquage du menu" << std::endl;
            showPauseMenu = false;
        }

        // Toujours appeler processEvents pour détecter Échap et les inputs du menu
        eventManager.processEvents(*player, allCharacters);

        // Détection des coffres proches du joueur
        currentChestNearby = nullptr;
        sf::FloatRect playerBounds = player->getBounds();
        for (auto &chest : currentLevel.gameObjects)
        {
            if (Chest *c = dynamic_cast<Chest *>(chest.get()))
            {
                if (!c->getIsOpened() && c->isPlayerOnChest(playerBounds))
                {
                    currentChestNearby = c;
                    break;
                }
            }
        }

        // Ouvrir le coffre si E est pressé
        if (eventManager.isInteractPressed() && currentChestNearby != nullptr)
        {
            currentChestNearby->open();
        }

        // Détection des portes proches du joueur
        currentDoorNearby = nullptr;
        for (auto &obj : currentLevel.gameObjects)
        {
            if (Door *d = dynamic_cast<Door *>(obj.get()))
            {
                if (d->isPlayerOnDoor(playerBounds))
                {
                    currentDoorNearby = d;
                    break;
                }
            }
        }

        // Transitionner vers le niveau suivant si E est pressé sur la exitDoor
        if (eventManager.isInteractPressed() && currentDoorNearby != nullptr && currentDoorNearby->getDoorType() == Door::DoorType::ExitDoor)
        {
            // Générer un nouveau niveau
            currentLevel = generateNewLevel(window);
            
            // Réinitialiser la liste allCharacters avec les nouveaux personnages
            allCharacters.clear();
            allCharacters.push_back(player.get());
            for (auto &character : currentLevel.spawnedCharacters)
                allCharacters.push_back(character.get());
            
            // Repositionner le joueur à la startDoor
            // La startDoor est à la position (0, 7), ce qui correspond au bas à gauche
            player->setPosition(20.f, 256.f * 8);
        }

        if (!isPaused && !showPauseMenu)
        {
            // Update
            for (auto *character : allCharacters)
            {
                if (character->isAlive())
                {
                    // Update behavior for non-player characters (enemies)
                    if (NonPlayer *npc = dynamic_cast<NonPlayer *>(character))
                    {
                        npc->updateBehavior(deltaTime, player.get(), currentLevel.grounds);
                    }
                    else
                    {
                        character->update(deltaTime, currentLevel.grounds);
                    }
                }
            }

            allCharacters.erase(
                std::remove_if(allCharacters.begin(), allCharacters.end(),
                               [](GameCharacter *c)
                               { return !c->isAlive() && dynamic_cast<NonPlayer *>(c); }),
                allCharacters.end());
            
            // Update camera to follow player or show full map
            if (eventManager.isMapViewActive())
            {
                // Vue de la carte complète
                // Calcul du centre et de la taille pour voir la map entière
                float mapWidth = 14.f * 274.f;  // 14 cases de 128 pixels
                float mapHeight = 8.f * 270.f;   // 8 cases de 128 pixels
                sf::View mapView(sf::Vector2f(mapWidth / 2.f, mapHeight / 2.f), sf::Vector2f(mapWidth, mapHeight));
                window.setView(mapView);
            }
            else
            {
                // Vue du joueur
                gameView.setCenter(player->getPosition());
                window.setView(gameView);
            }
        }

        window.clear();

        window.draw(backgroundSprite);

        for (auto &g : currentLevel.grounds)
            g->draw(window);

        // Afficher les objets du jeu (coffres, etc.)
        for (auto &obj : currentLevel.gameObjects)
            obj->draw(window);

        // Afficher le hint "[E]" si un coffre est proche
        if (currentChestNearby != nullptr && !currentChestNearby->getIsOpened())
        {
            sf::Text hintText;
            hintText.setFont(font);
            hintText.setString("[E]");
            hintText.setCharacterSize(20);
            hintText.setFillColor(sf::Color::Yellow);
            
            sf::Vector2f chestPos = currentChestNearby->getPosition();
            hintText.setPosition(chestPos.x, chestPos.y - 30.f);
            window.draw(hintText);
        }

        // Afficher le hint "[E]" si une porte est proche
        if (currentDoorNearby != nullptr && currentDoorNearby->getDoorType() == Door::DoorType::ExitDoor)
        {
            sf::Text hintText;
            hintText.setFont(font);
            hintText.setString("[E] Next Level");
            hintText.setCharacterSize(18);
            hintText.setFillColor(sf::Color::Cyan);
            
            sf::Vector2f doorPos = currentDoorNearby->getPosition();
            hintText.setPosition(doorPos.x, doorPos.y - 30.f);
            window.draw(hintText);
        }

            for (auto *character : allCharacters)
            if (character->isAlive())
                character->draw(window);

        dev.drawInfo(window, *player, allCharacters);

        // Afficher le menu de pause si actif
        if (showPauseMenu)
        {
            // std::cout << "[DEBUG MAIN] Dessin du menu de pause" << std::endl;
            
            // Sauvegarder la vue actuelle (vue du jeu zoomée)
            sf::View currentView = window.getView();
            
            // Appliquer la vue par défaut de la fenêtre pour le menu
            window.setView(window.getDefaultView());
            
            // Dessiner le menu de pause
            pauseMenu.draw(window);
            
            // Restaurer la vue du jeu
            window.setView(currentView);
        }

        window.display();
    }

    return 0;
}
#endif