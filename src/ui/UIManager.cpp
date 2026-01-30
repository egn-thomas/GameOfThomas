#include "UIManager.hpp"
#include "../characters/GameCharacter.hpp"
#include <sstream>
#include <iostream>
#include <filesystem>

UIManager::UIManager()
{
    // Try multiple possible paths for the font
    std::vector<std::string> fontPaths = {
        "../src/assets/fonts/RobotoMono-Regular.ttf",
        "src/assets/fonts/RobotoMono-Regular.ttf",
        "assets/fonts/RobotoMono-Regular.ttf",
        "../../src/assets/fonts/RobotoMono-Regular.ttf"
    };

    fontLoaded = false;
    for (const auto& path : fontPaths)
    {
        if (font.loadFromFile(path))
        {
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded)
    {
        std::cerr << "Failed to load font for UIManager (tried " << fontPaths.size() << " paths)" << std::endl;
    }
}

void UIManager::drawHealthBars(sf::RenderWindow& window, const std::vector<GameCharacter*>& allCharacters, GameCharacter* playerPtr)
{
    for (auto character : allCharacters)
    {
        // Skip the player
        if (character == playerPtr)
            continue;

        // Draw health bar above the character's head
        sf::Vector2f characterPos = character->getPosition();
        float barX = characterPos.x + character->getBounds().width / 2.f - barWidth / 2.f;
        float barY = characterPos.y + barOffsetY;

        drawHealthBar(window, barX, barY, character->getHp(), character->getMaxHp());
    }
}

void UIManager::drawPlayerHealthBar(sf::RenderWindow& window, const GameCharacter& player)
{
    // Draw player health bar in bottom left
    drawHealthBar(window, 10.f, window.getSize().y - 30.f, player.getHp(), player.getMaxHp());

    // Draw player HP text
    if (fontLoaded)
    {
        sf::Text hpText;
        hpText.setFont(font);
        hpText.setCharacterSize(32);
        hpText.setFillColor(sf::Color::White);

        std::stringstream ss;
        ss << "HP: " << player.getHp() << " / " << player.getMaxHp();
        hpText.setString(ss.str());
        hpText.setPosition(10.f, window.getSize().y - 50.f);

        window.draw(hpText);
    }
}

void UIManager::drawPlayerMana(sf::RenderWindow& window, const GameCharacter& player)
{
    if (!fontLoaded)
        return;

    sf::Text manaText;
    manaText.setFont(font);
    manaText.setCharacterSize(32);
    manaText.setFillColor(sf::Color::Cyan);

    std::stringstream ss;
    ss << "Mana: " << player.getMana() << " / " << player.getMaxMana();
    manaText.setString(ss.str());
    manaText.setPosition(10.f, window.getSize().y - 30.f);

    window.draw(manaText);
}

void UIManager::drawHealthBar(sf::RenderWindow& window, float x, float y, int currentHP, int maxHP)
{
    // Background bar (red/empty)
    sf::RectangleShape backgroundBar(sf::Vector2f(barWidth, barHeight));
    backgroundBar.setPosition(x, y);
    backgroundBar.setFillColor(sf::Color::Red);
    window.draw(backgroundBar);

    // Health bar (green/filled based on ratio)
    if (maxHP > 0)
    {
        float healthRatio = static_cast<float>(currentHP) / static_cast<float>(maxHP);
        float filledWidth = barWidth * healthRatio;

        sf::RectangleShape healthBar(sf::Vector2f(filledWidth, barHeight));
        healthBar.setPosition(x, y);
        healthBar.setFillColor(sf::Color::Green);
        window.draw(healthBar);
    }

    // Border
    sf::RectangleShape border(sf::Vector2f(barWidth, barHeight));
    border.setPosition(x, y);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(0.5f);
    window.draw(border);
}

void UIManager::drawPlayerHUD(sf::RenderWindow& window, const GameCharacter& player, int currentLevel)
{
    // Save current view to restore it after rendering UI in screen space
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    const float hudX = 20.f;
    float hudY = window.getSize().y - 280.f;
    const float lineHeight = 65.f;
    const float barY = hudY + 15.f;

    if (!fontLoaded)
    {
        window.setView(currentView);
        return;
    }

    // --- HP Bar ---
    sf::Text hpLabel;
    hpLabel.setFont(font);
    hpLabel.setString("HP");
    hpLabel.setCharacterSize(38);
    hpLabel.setFillColor(sf::Color::White);
    hpLabel.setPosition(hudX, hudY);
    window.draw(hpLabel);

    // Draw HP bar directly with large size for HUD (not using drawHealthBar for enemy bars)
    const float hudBarWidth = 120.f;
    const float hudBarHeight = 15.f;
    
    sf::RectangleShape hpBackgroundBar(sf::Vector2f(hudBarWidth, hudBarHeight));
    hpBackgroundBar.setPosition(hudX + 180.f, barY);
    hpBackgroundBar.setFillColor(sf::Color::Red);
    window.draw(hpBackgroundBar);

    if (player.getMaxHp() > 0)
    {
        float healthRatio = static_cast<float>(player.getHp()) / static_cast<float>(player.getMaxHp());
        float filledWidth = hudBarWidth * healthRatio;

        sf::RectangleShape hpBar(sf::Vector2f(filledWidth, hudBarHeight));
        hpBar.setPosition(hudX + 180.f, barY);
        hpBar.setFillColor(sf::Color::Green);
        window.draw(hpBar);
    }

    sf::RectangleShape hpBorder(sf::Vector2f(hudBarWidth, hudBarHeight));
    hpBorder.setPosition(hudX + 180.f, barY);
    hpBorder.setFillColor(sf::Color::Transparent);
    hpBorder.setOutlineColor(sf::Color::White);
    hpBorder.setOutlineThickness(1.5f);
    window.draw(hpBorder);

    // --- HP Text ---
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(38);
    hpText.setFillColor(sf::Color::White);
    {
        std::stringstream ss;
        ss << player.getHp() << "/" << player.getMaxHp();
        hpText.setString(ss.str());
    }
    hpText.setPosition(hudX + 320.f, hudY);
    window.draw(hpText);

    // --- Mana Bar ---
    hudY += lineHeight;
    sf::Text manaLabel;
    manaLabel.setFont(font);
    manaLabel.setString("Mana");
    manaLabel.setCharacterSize(38);
    manaLabel.setFillColor(sf::Color::Cyan);
    manaLabel.setPosition(hudX, hudY);
    window.draw(manaLabel);

    // Draw mana bar (cyan color)
    sf::RectangleShape manaBackgroundBar(sf::Vector2f(hudBarWidth, hudBarHeight));
    manaBackgroundBar.setPosition(hudX + 180.f, hudY + 15.f);
    manaBackgroundBar.setFillColor(sf::Color::Blue);
    window.draw(manaBackgroundBar);

    if (player.getMaxMana() > 0)
    {
        float manaRatio = static_cast<float>(player.getMana()) / static_cast<float>(player.getMaxMana());
        float filledWidth = hudBarWidth * manaRatio;

        sf::RectangleShape manaBar(sf::Vector2f(filledWidth, hudBarHeight));
        manaBar.setPosition(hudX + 180.f, hudY + 15.f);
        manaBar.setFillColor(sf::Color::Cyan);
        window.draw(manaBar);
    }

    sf::RectangleShape manaBorder(sf::Vector2f(hudBarWidth, hudBarHeight));
    manaBorder.setPosition(hudX + 180.f, hudY + 15.f);
    manaBorder.setFillColor(sf::Color::Transparent);
    manaBorder.setOutlineColor(sf::Color::White);
    manaBorder.setOutlineThickness(1.5f);
    window.draw(manaBorder);

    // --- Mana Text ---
    sf::Text manaText;
    manaText.setFont(font);
    manaText.setCharacterSize(38);
    manaText.setFillColor(sf::Color::White);
    {
        std::stringstream ss;
        ss << player.getMana() << "/" << player.getMaxMana();
        manaText.setString(ss.str());
    }
    manaText.setPosition(hudX + 320.f, hudY);
    window.draw(manaText);

    // --- Endurance Bar ---
    hudY += lineHeight;
    sf::Text enduranceLabel;
    enduranceLabel.setFont(font);
    enduranceLabel.setString("Stamina");
    enduranceLabel.setCharacterSize(38);
    enduranceLabel.setFillColor(sf::Color::Yellow);
    enduranceLabel.setPosition(hudX, hudY);
    window.draw(enduranceLabel);

    // Draw endurance bar (yellow color)
    sf::RectangleShape enduranceBackgroundBar(sf::Vector2f(hudBarWidth, hudBarHeight));
    enduranceBackgroundBar.setPosition(hudX + 180.f, hudY + 15.f);
    enduranceBackgroundBar.setFillColor(sf::Color::Black);
    window.draw(enduranceBackgroundBar);

    if (player.getMaxEndurance() > 0)
    {
        float enduranceRatio = static_cast<float>(player.getEndurance()) / static_cast<float>(player.getMaxEndurance());
        float filledWidth = hudBarWidth * enduranceRatio;

        sf::RectangleShape enduranceBar(sf::Vector2f(filledWidth, hudBarHeight));
        enduranceBar.setPosition(hudX + 180.f, hudY + 15.f);
        enduranceBar.setFillColor(sf::Color::Yellow);
        window.draw(enduranceBar);
    }

    sf::RectangleShape enduranceBorder(sf::Vector2f(hudBarWidth, hudBarHeight));
    enduranceBorder.setPosition(hudX + 180.f, hudY + 15.f);
    enduranceBorder.setFillColor(sf::Color::Transparent);
    enduranceBorder.setOutlineColor(sf::Color::White);
    enduranceBorder.setOutlineThickness(1.5f);
    window.draw(enduranceBorder);

    // --- Endurance Text ---
    sf::Text enduranceText;
    enduranceText.setFont(font);
    enduranceText.setCharacterSize(38);
    enduranceText.setFillColor(sf::Color::White);
    {
        std::stringstream ss;
        ss << player.getEndurance() << "/" << player.getMaxEndurance();
        enduranceText.setString(ss.str());
    }
    enduranceText.setPosition(hudX + 320.f, hudY);
    window.draw(enduranceText);

    // --- Level ---
    hudY += lineHeight;
    sf::Text levelLabel;
    levelLabel.setFont(font);
    levelLabel.setCharacterSize(38);
    levelLabel.setFillColor(sf::Color::Magenta);
    {
        std::stringstream ss;
        ss << "Level " << currentLevel;
        levelLabel.setString(ss.str());
    }
    levelLabel.setPosition(hudX, hudY);
    window.draw(levelLabel);

    // Restore view
    window.setView(currentView);
}