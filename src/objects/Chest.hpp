#pragma once
#include "Object.hpp"
#include <memory>

/**
 * @brief Classe représentant un coffre
 * Hérite de Object et ajoute les propriétés spécifiques au coffre
 */
class Chest : public Object
{
private:
    bool isOpened = false;  // État du coffre

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
     * À développer plus tard avec la logique de récompense
     */
    virtual void open();
};
