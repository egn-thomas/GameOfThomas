#include "ExitBlock.hpp"
#include "../objects/Chest.hpp"
#include "../objects/Door.hpp"
#include "../factories/CharacterFactory.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

ExitBlock::ExitBlock(BlockType type, int posx, int posy, sf::RenderWindow &window)
    : mask_(type), posx_(posx), posy_(posy), window_(&window)
{
    grounds_ = buildGrounds(mask_, posx_, posy_, window);
}

void ExitBlock::draw(sf::RenderWindow &window, int x, int y, int cellSize)
{
    // Minimal visualisation for the tile area (transparent)
    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
    rect.setPosition(static_cast<float>(x), static_cast<float>(y));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color(150, 150, 150, 50));
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    // Optionally draw the grounds inside the block for debug (comment out if double-drawn)
    for (auto &g : grounds_)
        g->draw(window);
}

std::vector<std::unique_ptr<Ground>> ExitBlock::createGrounds()
{
    return std::move(grounds_);
}

std::vector<std::unique_ptr<Ground>> ExitBlock::buildGrounds(BlockType mask, int posx, int posy, sf::RenderWindow &window)
{
    std::vector<std::unique_ptr<Ground>> grounds;

    float tileSizeX = static_cast<float>(window.getSize().x) / static_cast<float>(GRID_COLS);
    float tileSizeY = static_cast<float>(window.getSize().y) / static_cast<float>(GRID_ROWS);
    // Node positions are 0-based (x in [0..GRID_COLS-1]) so multiply directly
    float tilesPositionX = tileSizeX * static_cast<float>(posx);
    float tilesPositionY = tileSizeY * static_cast<float>(posy);

    const float floorThickness = 16.f;
    const float wallThickness = 16.f;
    const float openingWidth = 96.f;

    // Floor: placed at bottom of tile. Skip floor if there's an exit downward
    float floorPosX = tilesPositionX;
    float floorPosY = tilesPositionY + tileSizeY - floorThickness;
    if (!(mask & BOTTOM_EXIT))
    {
        grounds.emplace_back(std::make_unique<TexturedGround>(floorPosX, floorPosY + 8, tileSizeX, floorThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") floor at " << floorPosX << "," << floorPosY << " size " << tileSizeX << "," << floorThickness << "\n";
    }
    else
    {
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") BOTTOM_EXIT -> skipped floor to allow passage\n";
    }
    std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") floor at " << floorPosX << "," << floorPosY << " size " << tileSizeX << "," << floorThickness << "\n";

    // Left wall (if LEFT_EXIT not set)
    if (!(mask & LEFT_EXIT))
    {
        float x = tilesPositionX;
        float y = tilesPositionY;
        grounds.emplace_back(std::make_unique<TexturedGround>(x - 8, y, wallThickness, tileSizeY, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") left wall at " << x << "," << y << " size " << wallThickness << "," << tileSizeY << "\n";
    }
    else
    {
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") LEFT_EXIT (opening) skipped left wall\n";
    }

    // Right wall (if RIGHT_EXIT not set)
    if (!(mask & RIGHT_EXIT))
    {
        float x = tilesPositionX + tileSizeX - wallThickness;
        float y = tilesPositionY;
        grounds.emplace_back(std::make_unique<TexturedGround>(x + 8, y, wallThickness, tileSizeY, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") right wall at " << x << "," << y << " size " << wallThickness << "," << tileSizeY << "\n";
    }
    else
    {
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") RIGHT_EXIT (opening) skipped right wall\n";
    }

    // Top wall (if TOP_EXIT not set)
    if (!(mask & TOP_EXIT))
    {
        float x = tilesPositionX;
        float y = tilesPositionY;
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y - 8, tileSizeX, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") top wall at " << x << "," << y << " size " << tileSizeX << "," << wallThickness << "\n";
    }
    else
    {
        // La sortie ne fait pas la largeur totale du mur, on laisse un peu de mur de chaque côté pour une ouverture de 100 pixels
        float sideWidth = (tileSizeX - openingWidth) / 2.f;
        grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX, tilesPositionY - 8, sideWidth, wallThickness, *TexturedGround::getDefaultTexture()));
        grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX + sideWidth + openingWidth, tilesPositionY - 8, sideWidth, wallThickness, *TexturedGround::getDefaultTexture()));

        // If there is a top exit, place a ladder centered in the opening
        float ladderX = tilesPositionX + sideWidth;
        float ladderY = tilesPositionY - floorThickness;
        grounds.emplace_back(std::make_unique<LadderGround>(ladderX, ladderY, openingWidth, tileSizeY + floorThickness, *LadderGround::getDefaultTexture()));

        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") TOP_EXIT (opening) skipped top wall\n";
    }

    // Bottom wall (if BOTTOM_EXIT not set)
    if (!(mask & BOTTOM_EXIT))
    {
        float x = tilesPositionX;
        float y = tilesPositionY + tileSizeY - wallThickness;
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y + 8, tileSizeX, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") bottom wall at " << x << "," << y << " size " << tileSizeX << "," << wallThickness << "\n";
    }
    else
    {
        // La sortie ne fait pas la largeur totale du mur, on laisse un peu de mur de chaque côté pour une ouverture de 100 pixels
        float sideWidthB = (tileSizeX - openingWidth) / 2.f;
        grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX, tilesPositionY + tileSizeY - wallThickness + 8, sideWidthB, wallThickness, *TexturedGround::getDefaultTexture()));
        grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX + sideWidthB + openingWidth, tilesPositionY + tileSizeY - wallThickness + 8, sideWidthB, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") BOTTOM_EXIT (opening) skipped bottom wall\n";
    }

    return grounds;
}

std::vector<std::unique_ptr<Object>> ExitBlock::createObjects()
{
    std::vector<std::unique_ptr<Object>> objects;

    // Ajouter la porte de départ en bas à gauche (0, 7)
    if (posx_ == 0 && posy_ == 7)
    {
        float tileSizeX = static_cast<float>(window_->getSize().x) / static_cast<float>(GRID_COLS);
        float tileSizeY = static_cast<float>(window_->getSize().y) / static_cast<float>(GRID_ROWS);

        float tilesPositionX = tileSizeX * static_cast<float>(posx_);
        float tilesPositionY = tileSizeY * static_cast<float>(posy_);

        float posX = tilesPositionX + 8.f; // Slight offset from left wall
        float posY = tilesPositionY + 8.f; // Slight offset from bottom wall

        sf::Vector2f doorPosition(posX, posY);
        auto doorTexture = std::make_shared<sf::Texture>();
        if (!doorTexture->loadFromFile("src/assets/images/startDoor.png"))
        {
            // Fallback to absolute path if relative path fails
            doorTexture->loadFromFile("/home/thomas/Documents/GitHub/GameOfThomas/src/assets/images/startDoor.png");
        }
        objects.emplace_back(std::make_unique<Door>(doorPosition, doorTexture, Door::DoorType::StartDoor));
    }

    // Ajouter la porte de sortie en haut à droite (13, 0)
    if (posx_ == 13 && posy_ == 0)
    {
        float tileSizeX = static_cast<float>(window_->getSize().x) / static_cast<float>(GRID_COLS);
        float tileSizeY = static_cast<float>(window_->getSize().y) / static_cast<float>(GRID_ROWS);

        float tilesPositionX = tileSizeX * static_cast<float>(posx_);
        float tilesPositionY = tileSizeY * static_cast<float>(posy_);

        float posX = tilesPositionX + 8.f; // Slight offset from right wall
        float posY = tilesPositionY + 8.f; // Slight offset from top wall

        sf::Vector2f doorPosition(posX, posY);
        auto doorTexture = std::make_shared<sf::Texture>();
        if (!doorTexture->loadFromFile("src/assets/images/exitDoor.png"))
        {
            // Fallback to absolute path if relative path fails
            doorTexture->loadFromFile("/home/thomas/Documents/GitHub/GameOfThomas/src/assets/images/exitDoor.png");
        }
        objects.emplace_back(std::make_unique<Door>(doorPosition, doorTexture, Door::DoorType::ExitDoor));
    }

    // Vérifier qu'il n'y a pas de sorties en haut ou en bas
    if ((mask_ & TOP_EXIT) || (mask_ & BOTTOM_EXIT))
    {
        return objects; // Pas de coffre si sorties en haut ou bas
    }

    // Générer le coffre selon le pourcentage de chance
    int chance = std::rand() % 100;
    if (chance < static_cast<int>(CHEST_SPAWN_CHANCE))
    {
        // Calculer la position en utilisant la même méthode que buildGrounds()
        float tileSizeX = static_cast<float>(window_->getSize().x) / static_cast<float>(GRID_COLS);
        float tileSizeY = static_cast<float>(window_->getSize().y) / static_cast<float>(GRID_ROWS);

        float tilesPositionX = tileSizeX * static_cast<float>(posx_);
        float tilesPositionY = tileSizeY * static_cast<float>(posy_);

        float wallThickness = 8.f;
        // Le coffre fait 32x32 pixels mis à l'échelle 4x = 128x128 pixels
        float chestWidth = 32.f * 4.f;  // 128 pixels
        float chestHeight = 32.f * 4.f; // 128 pixels

        // Centrer le coffre dans le bloc
        float posX = tilesPositionX + (tileSizeX - chestWidth) / 2.f;
        float posY = tilesPositionY + tileSizeY - chestHeight - wallThickness; // Posé sur le sol

        sf::Vector2f chestPosition(posX, posY);
        std::shared_ptr<sf::Texture> chestTextureClose = std::make_shared<sf::Texture>();
        std::shared_ptr<sf::Texture> chestTextureOpen = std::make_shared<sf::Texture>();
        if (!chestTextureClose->loadFromFile("../src/assets/images/chest.png"))
        {
            std::cerr << "Failed to load chest texture!" << std::endl;
            return objects;
        }
        if (!chestTextureOpen->loadFromFile("../src/assets/images/chestOpened.png"))
        {
            std::cerr << "Failed to load chest open texture!" << std::endl;
            return objects;
        }

        objects.emplace_back(std::make_unique<Chest>(chestPosition, chestTextureClose));
        std::cout << "Chest created in ExitBlock at grid(" << posx_ << ", " << posy_ << ") with world position (" << posX << ", " << posY << ")" << std::endl;
    }

    return objects;
}

std::vector<std::unique_ptr<GameCharacter>> ExitBlock::createCharacters()
{
    std::vector<std::unique_ptr<GameCharacter>> characters;

    // Vérifier qu'il n'y a pas de sorties en haut ou en bas
    if ((mask_ & TOP_EXIT) || (mask_ & BOTTOM_EXIT))
    {
        return characters; // Pas de mobs si sorties en haut ou bas
    }

    // Générer un mob selon le pourcentage de chance
    int chance = std::rand() % 100;
    if (chance < static_cast<int>(CANDLE_SPAWN_CHANCE))
    {
        // Calculer la position en utilisant la même méthode que buildGrounds()
        float tileSizeX = static_cast<float>(window_->getSize().x) / static_cast<float>(GRID_COLS);
        float tileSizeY = static_cast<float>(window_->getSize().y) / static_cast<float>(GRID_ROWS);

        float tilesPositionX = tileSizeX * static_cast<float>(posx_);
        float tilesPositionY = tileSizeY * static_cast<float>(posy_);

        // Centrer le mob dans le bloc
        float posX = tilesPositionX + tileSizeX / 2.f;
        float posY = tilesPositionY + tileSizeY / 2.f;

        sf::Vector2f mobPosition(posX, posY);

        auto mob = CharacterFactory::createCandle(mobPosition);
        std::cout << "Mob (Candle) created in ExitBlock at grid(" << posx_ << ", " << posy_ << ") with world position (" << posX << ", " << posY << ")" << std::endl;

        characters.push_back(std::move(mob));
    }

    return characters;
}