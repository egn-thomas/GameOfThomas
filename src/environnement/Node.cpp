#include "Node.hpp"
#include <iostream>

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

/**
 * @brief Traite l'ajout d'un objet dans le noeud. Choisis si le node peur accepter l'objet. Et gère les probabilités d'apparition.
 * @note Pour l'instant cette fonction ne permet d'ajouter que des Chest. qui est la seule classe dérivée de Object pour l'instant.
 */
void Node::processAddingObject()
{
    if (this->top == false || this->bottom == false)
    {
        // Ne pas ajouter d'objet si le node a des ouvertures en haut ou en bas
        return;
    } else {
        // 20% de chance d'ajouter un coffre
        int chance = std::rand() % 100;
        if (chance < 20)
        {
            // Créer et ajouter un coffre à la position du node
            // Une tuile fait 128 pixels (32 pixels * 4x échelle)
            // Le coffre fait 32x32 pixels mis à l'échelle 4x = 128x128 pixels
            float tileWidth = 128.f;
            float tileHeight = 128.f;
            float chestWidth = 32.f * 4.f;  // 128 pixels
            float chestHeight = 32.f * 4.f; // 128 pixels
            
            // Centrer horizontalement et coller au sol
            float posX = xPos * tileWidth + (tileWidth - chestWidth) / 2.f;
            float posY = yPos * tileHeight + (tileHeight - chestHeight);
            
            sf::Vector2f objPosition(posX, posY);
            std::shared_ptr<sf::Texture> chestTexture = std::make_shared<sf::Texture>();
            if (!chestTexture->loadFromFile("../src/assets/images/chest.png"))
            {
                std::cerr << "Failed to load chest texture!" << std::endl;
                return;
            }
            Object chest("Chest", objPosition, chestTexture);
            this->addObject(chest);
            std::cout << "Chest created at Node (" << xPos << ", " << yPos << ") at position (" << posX << ", " << posY << ")" << std::endl;
        }
    }
}

void Node::addObject(const Object &obj)
{
    objectsInNode.push_back(obj);
}