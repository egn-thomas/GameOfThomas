#include "ExitBlock.hpp"
#include <iostream>

ExitBlock::ExitBlock(BlockType type, int posx, int posy, sf::RenderWindow &window)
    : mask_(type), posx_(posx), posy_(posy)
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

    const float floorThickness = 8.f;
    const float wallThickness = 8.f;
    const float openingWidth = 96.f;

    // Floor: placed at bottom of tile. Skip floor if there's an exit downward
    float floorPosX = tilesPositionX;
    float floorPosY = tilesPositionY + tileSizeY - floorThickness;
    if (!(mask & BOTTOM_EXIT))
    {
        grounds.emplace_back(std::make_unique<TexturedGround>(floorPosX, floorPosY, tileSizeX, floorThickness, *TexturedGround::getDefaultTexture()));
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
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y, wallThickness, tileSizeY, *TexturedGround::getDefaultTexture()));
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
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y, wallThickness, tileSizeY, *TexturedGround::getDefaultTexture()));
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
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y, tileSizeX, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") top wall at " << x << "," << y << " size " << tileSizeX << "," << wallThickness << "\n";
    }
    else
    {
        // La sortie ne fait pas la largeur totale du mur, on laisse un peu de mur de chaque côté pour une ouverture de 100 pixels
    float sideWidth = (tileSizeX - openingWidth) / 2.f;
    grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX, tilesPositionY, sideWidth, wallThickness, *TexturedGround::getDefaultTexture()));
    grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX + sideWidth + openingWidth, tilesPositionY, sideWidth, wallThickness, *TexturedGround::getDefaultTexture()));

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
        grounds.emplace_back(std::make_unique<TexturedGround>(x, y, tileSizeX, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") bottom wall at " << x << "," << y << " size " << tileSizeX << "," << wallThickness << "\n";
    }
    else
    {
        // La sortie ne fait pas la largeur totale du mur, on laisse un peu de mur de chaque côté pour une ouverture de 100 pixels
    float sideWidthB = (tileSizeX - openingWidth) / 2.f;
    grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX, tilesPositionY + tileSizeY - wallThickness, sideWidthB, wallThickness, *TexturedGround::getDefaultTexture()));
    grounds.emplace_back(std::make_unique<TexturedGround>(tilesPositionX + sideWidthB + openingWidth, tilesPositionY + tileSizeY - wallThickness, sideWidthB, wallThickness, *TexturedGround::getDefaultTexture()));
        std::cerr << "ExitBlock: tile(" << posx << "," << posy << ") BOTTOM_EXIT (opening) skipped bottom wall\n";
    }

    return grounds;
}
