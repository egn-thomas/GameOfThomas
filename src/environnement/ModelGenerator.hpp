#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP
#include <SFML/Graphics.hpp>
#include "Node.hpp" 

/**
 * @brief Classe pour générer un modèle de labyrinthe
 * Utilise une grille de nœuds pour créer des chemins
 */
class ModelGenerator
{
protected:
    int width;
    int height;
    std::vector<Node*> grid;
public:
    ModelGenerator(int width, int height);
    void generateMaze(int width, int height);

    const std::vector<Node *> &getGrid() const;
    int getWidth() const;
    int getHeight() const;
};
#endif