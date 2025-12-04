#include "Node.hpp"

/**
 * @brief Constructeur d'un Node
 *
 * @param xPos Le numero de colonnes du Node
 * @param yPos Le numero de ligne du Node
 */
Node::Node(int id, int xPos, int yPos) : id(id), xPos(xPos), yPos(yPos)
{
}

/**
 * @brief Ajoute un Node a sa liste de voisins
 *
 * @param n Le node à ajouter
 */
void Node::addNeighbors(Node *n)
{
    if (this->isPositionsNeighbors(n))
        this->neighbors.push_back(n);
}

/**
 * @brief retourne true si le Node passé en paramètre et voisin de ce Node.
 */
bool Node::isPositionsNeighbors(Node *n)
{
    if ((this->xPos == n->xPos && (this->yPos == n->yPos + 1 || this->yPos == n->yPos - 1)) || (this->yPos == n->yPos && (this->xPos == n->xPos + 1 || this->xPos == n->xPos - 1)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief retire un mur avec le node passé en paramètre.
 */
void Node::removeWallWith(Node *other)
{
    if (xPos == other->xPos)
    {
        if (yPos == other->yPos + 1)
        {
            if (!forcedTop && !other->forcedBottom)
            {
                top = false;
                other->bottom = false;
            }
        }
        else if (yPos == other->yPos - 1)
        {
            if (!forcedBottom && !other->forcedTop)
            {
                bottom = false;
                other->top = false;
            }
        }
    }
    else if (yPos == other->yPos)
    {
        if (xPos == other->xPos + 1)
        {
            if (!forcedLeft && !other->forcedRight)
            {
                left = false;
                other->right = false;
            }
        }
        else if (xPos == other->xPos - 1)
        {
            if (!forcedRight && !other->forcedLeft)
            {
                right = false;
                other->left = false;
            }
        }
    }
}

bool Node::isVisited()
{
    return (this->visited);
}

void Node::setVisited(bool a)
{
    this->visited = a;
}

std::vector<Node *> Node::getNeighbors()
{
    return (this->neighbors);
}