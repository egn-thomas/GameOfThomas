#pragma once
#include <SFML/Graphics.hpp>
#include "characters/GameCharacter.hpp"
#include <string>

class DevMode {
public:
    explicit DevMode(bool active = true);

    // Affiche les infos du joueur
    void drawInfo(sf::RenderWindow& window, const GameCharacter& player, std::vector<GameCharacter *> allCharacters);

    // Dessine des overlays de debug (hitboxes, bounds des grounds)
    void drawDebugOverlays(sf::RenderWindow& window, const GameCharacter& player, const std::vector<std::unique_ptr<Ground>>& grounds, std::vector<GameCharacter *> allCharacters);

    // Activer / désactiver le mode dev
    void setActive(bool active);
    bool isActive() const;

private:
    bool active;

    // Pour dessiner du texte
    sf::Font font;
    sf::Text text;
    bool fontLoaded = false;

    // Formatage pratique
    std::string formatVector(const sf::Vector2f& v) const;
};