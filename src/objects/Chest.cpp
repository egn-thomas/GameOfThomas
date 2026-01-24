#include "Chest.hpp"

/**
 * @brief Constructeur de Chest
 */
Chest::Chest(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture)
    : Object("Chest", position, texture)
{
    // Configuration spécifique du coffre si besoin
}

/**
 * @brief Vérifie si le coffre est ouvert
 */
bool Chest::getIsOpened() const
{
    return isOpened;
}

/**
 * @brief Ouvre le coffre
 * Logique à implémenter plus tard
 */
void Chest::open()
{
    if (!isOpened)
    {
        isOpened = true;
        // À faire : ajouter des récompenses, sons, animations, etc.
    }
}
