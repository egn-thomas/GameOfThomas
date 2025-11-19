#include "ModelGenerator.hpp"
#include "Node.hpp"
#include <stack>
#include <algorithm>
#include <random>
#include <ctime>

/**
 * @brief Constructeur de la classe ModelGenerator.
 *
 * @param width Le nombre de colonnes de tuiles du niveau.
 * @param height Le nombre de lignes de tuiles du niveau.
 */
ModelGenerator::ModelGenerator(int width, int height)
{
    generateMaze(width, height);
}

/**
 * @brief Génère le Maze du niveau.
 *
 * @param width Le nombre de colonnes de tuiles du niveau.
 * @param height Le nombre de lignes de tuiles du niveau.
 */
void ModelGenerator::generateMaze(int w, int h)
{
    // Stocker largeur et hauteur
    width = w;
    height = h;

    // 1. Créer la grille de nodes
    grid.clear();
    grid.reserve(width * height);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int id = y * width + x;
            grid.push_back(new Node(id, x, y));
        }
    }

    // 2. Connecter les voisins (gauche, droite, haut, bas)
    for (auto n : grid)
    {
        for (auto m : grid)
        {
            n->addNeighbors(m);
        }
    }

    // 3. DFS Backtracking
    std::stack<Node *> stack;

    std::srand(std::time(nullptr));

    Node *start = grid[0]; // ou randomiser
    start->setVisited(true);
    stack.push(start);

    while (!stack.empty())
    {
        Node *current = stack.top();

        std::vector<Node *> unvisited;
        for (Node *n : current->getNeighbors())
            if (!n->isVisited())
                unvisited.push_back(n);

        if (unvisited.empty())
        {
            stack.pop();
        }
        else
        {
            Node *next = unvisited[std::rand() % unvisited.size()];

            current->removeWallWith(next);

            next->setVisited(true);
            stack.push(next);
        }
    }
}

const std::vector<Node*>& ModelGenerator::getGrid() const {
    return grid;
}

int ModelGenerator::getWidth() const {
    return width;
}

int ModelGenerator::getHeight() const {
    return height;
}
