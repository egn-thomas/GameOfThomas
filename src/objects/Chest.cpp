#include "Chest.hpp"

/**
 * @brief Constructeur de Chest
 */
Chest::Chest(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture)
    : Object("Chest", position, texture), closedTexture(texture)
{
    // Charger la texture du coffre ouvert
    openedTexture = std::make_shared<sf::Texture>();
    if (!openedTexture->loadFromFile("../src/assets/images/chestOpened.png"))
    {
        // Si le fichier n'existe pas, utiliser la même texture
        openedTexture = closedTexture;
    }
}

/**
 * @brief Vérifie si le coffre est ouvert
 */
bool Chest::getIsOpened() const
{
    return this->isOpened;
}

/**
 * @brief Ouvre le coffre
 */
void Chest::open()
{
    if (!this->isOpened)
    {
        this->isOpened = true;
        this->setTexture(true);  // Changer la texture pour l'état ouvert
    }
}

/**
 * @brief Définit la texture du coffre
 * @param opened true pour ouvert, false pour fermé
 */
void Chest::setTexture(bool opened)
{
    if (opened)
    {
        this->texture = openedTexture;
    }
    else
    {
        this->texture = closedTexture;
    }
    this->sprite.setTexture(*this->texture);
}

/**
 * @brief Vérifie si le joueur est sur le coffre (collision AABB)
 */
bool Chest::isPlayerOnChest(const sf::FloatRect &playerBounds) const
{
    return hitbox.intersects(playerBounds);
}
