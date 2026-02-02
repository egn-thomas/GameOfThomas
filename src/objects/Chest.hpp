#pragma once
#include "Object.hpp"
#include <memory>
#include <vector>

class Item;

/**
 * @brief Classe représentant un coffre
 * Hérite de Object et ajoute les propriétés spécifiques au coffre
 */
class Chest : public Object
{
private:
    bool isOpened = false;  // État du coffre
    std::shared_ptr<sf::Texture> closedTexture;
    std::shared_ptr<sf::Texture> openedTexture;

public:
    /**
     * @brief Constructeur de Chest
     * @param position Position du coffre dans le monde
     * @param texture Texture du coffre
     */
    Chest(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture);

    /**
     * @brief Vérifie si le coffre est ouvert
     */
    bool getIsOpened() const;

    /**
     * @brief Ouvre le coffre
     */
    void open();

    /**
     * @brief Définit la texture du coffre
     */
    void setTexture(bool opened);

    /**
     * @brief Génère le contenu du coffre en fonction du niveau de jeu.
     * @param level Le niveau de jeu actuel (1...N)
     * @return Vecteur d'items que contient ce coffre
     */
    std::vector<std::unique_ptr<Item>> generateLoot(int level) const;

    /**
     * @brief Vérifie si le joueur est sur le coffre
     */
    bool isPlayerOnChest(const sf::FloatRect &playerBounds) const;
};
