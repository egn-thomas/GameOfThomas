#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../environnement/Ground.hpp"

/**
 * @brief Classe abstraite représentant une tuile dans le jeu (zone du plateau).
 */
class Block
{
public:
    virtual ~Block() = default;
    virtual void draw(sf::RenderWindow& window, int x, int y, int cellSize) = 0;
    // Retourne les Ground associés à ce Block. Par convention la méthode
    // retourne un vecteur par valeur et transfère la propriété des Grounds
    // appelant (via std::move) si nécessaire.
    virtual std::vector<std::unique_ptr<Ground>> createGrounds() = 0;
};

#endif
