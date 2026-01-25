#pragma once
#include "Object.hpp"
#include <memory>

/**
 * @brief Classe représentant une porte
 * Hérite de Object et ajoute les propriétés spécifiques à une porte
 */
class Door : public Object
{
public:
    enum class DoorType {
        StartDoor,  // Porte de départ
        ExitDoor    // Porte de sortie
    };

private:
    DoorType doorType;

public:
    /**
     * @brief Constructeur de Door
     * @param position Position de la porte dans le monde
     * @param texture Texture de la porte
     * @param type Type de porte
     */
    Door(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture, DoorType type);

    /**
     * @brief Retourne le type de porte
     */
    DoorType getDoorType() const;

    /**
     * @brief Vérifie si le joueur est sur la porte (collision AABB)
     */
    bool isPlayerOnDoor(const sf::FloatRect &playerBounds) const;
};
