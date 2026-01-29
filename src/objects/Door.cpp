#include "Door.hpp"

/**
 * @brief Constructeur de Door
 */
Door::Door(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture, DoorType type)
    : Object("Door", position, texture), doorType(type)
{
    // Scaling pour les portes (plus petit que les autres objets)
    sprite.setScale(2.f, 2.f);
    // Redimensionner la hitbox pour correspondre au scaling (texture 128x128 * scale 2)
    hitbox = sf::FloatRect(position.x, position.y, 128.f * 2.f, 128.f * 2.f);
}

/**
 * @brief Retourne le type de porte
 */
Door::DoorType Door::getDoorType() const
{
    return doorType;
}

/**
 * @brief Vérifie si le joueur est sur la porte (collision AABB)
 */
bool Door::isPlayerOnDoor(const sf::FloatRect &playerBounds) const
{
    return hitbox.intersects(playerBounds);
}
