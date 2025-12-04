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

    // We'll enforce the game's corner clarity constraint, but do it AFTER
    // the DFS maze carving. Setting forced flags before the DFS can
    // accidentally partition the graph and make regions unreachable.
    // So we perform a standard randomized DFS first, then post-process
    // the generated walls to ensure the two corner constraints.

    // 3. DFS Backtracking
    std::stack<Node *> stack;

    std::srand(std::time(nullptr));

    Node *start = grid[(height - 1) * width + 0];
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

    // Post-process: enforce corner constraints without blocking the DFS.
    // Bottom-left (x=0,y=height-1) must only connect to its right neighbor.
    if (width >= 2)
    {
        Node *bottomLeft = grid[(height - 1) * width + 0];
        Node *rightOfBottomLeft = grid[(height - 1) * width + 1];

        // Close all walls on bottomLeft, then open only the right side.
        bottomLeft->top = true;
        bottomLeft->bottom = true;
        bottomLeft->left = true;
        bottomLeft->right = false;

        // Make sure the neighbor's corresponding wall is open too.
        rightOfBottomLeft->left = false;
    }

    // Top-right (x=width-1,y=0) must only connect to its left neighbor.
    if (width >= 2)
    {
        Node *topRight = grid[0 * width + (width - 1)];
        Node *leftOfTopRight = grid[0 * width + (width - 2)];

        // Close all walls on topRight, then open only the left side.
        topRight->top = true;
        topRight->right = true;
        topRight->bottom = true;
        topRight->left = false;

        // Ensure the neighbor's corresponding wall is open too.
        leftOfTopRight->right = false;
    }

    // Ensure pairwise symmetry of walls: if either side was opened during
    // generation, keep the opening. This fixes tiny mismatches caused by
    // manual post-processing while preserving open passages.
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Node *n = grid[y * width + x];
            // sync with node below
            if (y + 1 < height)
            {
                Node *below = grid[(y + 1) * width + x];
                bool wallPresent = n->bottom && below->top; // true if both sides think wall is present
                n->bottom = wallPresent;
                below->top = wallPresent;
            }
            // sync with node to the right
            if (x + 1 < width)
            {
                Node *r = grid[y * width + (x + 1)];
                bool wallPresent = n->right && r->left;
                n->right = wallPresent;
                r->left = wallPresent;
            }
        }
    }
}

const std::vector<Node *> &ModelGenerator::getGrid() const
{
    return grid;
}

int ModelGenerator::getWidth() const
{
    return width;
}

int ModelGenerator::getHeight() const
{
    return height;
}
