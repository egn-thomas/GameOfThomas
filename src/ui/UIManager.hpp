#include "InventoryMenu.hpp"
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

class GameCharacter;

class UIManager
{
public:
    UIManager();

    // Inventaire
    void drawInventoryMenu(sf::RenderWindow& window, const GameCharacter& character);
    InventoryMenu& getInventoryMenu();

    /**
     * @brief Dessine les barres de vie pour tous les personnages (sauf le joueur)
     * @param window La fenêtre de rendu
     * @param allCharacters Liste de tous les personnages
     * @param playerPtr Pointeur vers le joueur (pour l'exclure de l'affichage de barre)
     */
    void drawHealthBars(sf::RenderWindow& window, const std::vector<GameCharacter*>& allCharacters, GameCharacter* playerPtr);

    /**
     * @brief Dessine la barre de vie du joueur en bas à gauche
     * @param window La fenêtre de rendu
     * @param player Le joueur
     */
    void drawPlayerHealthBar(sf::RenderWindow& window, const GameCharacter& player);

    /**
     * @brief Dessine le mana du joueur
     * @param window La fenêtre de rendu
     * @param player Le joueur
     */
    void drawPlayerMana(sf::RenderWindow& window, const GameCharacter& player);

    /**
     * @brief Dessine le HUD complet du joueur en bas à gauche (HP, Mana, Endurance, Niveau)
     * @param window La fenêtre de rendu
     * @param player Le joueur
     * @param currentLevel Le niveau actuel
     */
    void drawPlayerHUD(sf::RenderWindow& window, const GameCharacter& player, int currentLevel);

    // --- Notifications éphémères ---
    // Ajoute une notification à afficher (droite de l'écran)
    void addNotification(const std::string &text, float duration = 3.f);
    // Met à jour le timer des notifications (à appeler chaque frame)
    void updateNotifications(float deltaTime);
    // Dessine les notifications à l'écran (vue par défaut)
    void drawNotifications(sf::RenderWindow& window);

private:
    sf::Font font;
    bool fontLoaded = false;

    InventoryMenu inventoryMenu;

    // Structure interne pour les notifications
    struct Notification { std::string text; float ttl; };
    std::vector<Notification> notifications;
    static constexpr float notificationWidth = 500.f;
    static constexpr float notificationMargin = 20.f;

    // Paramètres des barres de vie des ennemis (petites)
    static constexpr float barWidth = 40.f;
    static constexpr float barHeight = 5.f;   
    static constexpr float barOffsetY = -15.f; // Au-dessus de la tête

    /**
     * @brief Dessine une barre de vie à une position donnée
     * @param window La fenêtre de rendu
     * @param x Position X
     * @param y Position Y
     * @param currentHP Points de vie actuels
     * @param maxHP Points de vie maximum
     */
    void drawHealthBar(sf::RenderWindow& window, float x, float y, int currentHP, int maxHP);
};
