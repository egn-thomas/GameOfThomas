#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "../characters/GameCharacter.hpp"

class InventoryMenu {
public:
    InventoryMenu();
    void draw(sf::RenderWindow& window, const GameCharacter& character);
    void moveSelection(int direction); // -1: gauche, +1: droite
    int getSelectedSlot() const { return selectedSlot; }
    void resetSelection() { selectedSlot = 0; }

private:
    sf::Font font;
    int selectedSlot = 0;
    sf::Color unselectedColor = sf::Color(80,80,80,180);

    // Textures cache pour les sprites d'items
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureCache;

    // Helper: charge et met en cache une texture
    std::shared_ptr<sf::Texture> getTexture(const std::string& path);

    // Helper: crée une forme convexe approximant un rectangle arrondi
    sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius, int cornerSegments = 8);
};
