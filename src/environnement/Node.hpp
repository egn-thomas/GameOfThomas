#ifndef NODE_HPP
#define NODE_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>
#include "../objects/Object.hpp"

class Node
{
private:
    int id;
    std::vector<Node *> neighbors;

    int xPos, yPos;
    bool visited = false;

public:
    bool forcedTop = false;
    bool forcedBottom = false;
    bool forcedLeft = false;
    bool forcedRight = false;

    bool top = true;
    bool bottom = true;
    bool left = true;
    bool right = true;

    std::vector<Object> objectsInNode;

    Node(int id, int xPos, int yPos);
    void addNeighbors(Node *n);
    bool isPositionsNeighbors(Node *n);
    std::vector<Node *> getNeighbors();
    int getxPos() { return xPos; }
    int getyPos() { return yPos; }
    void removeWallWith(Node *other);
    bool isVisited();
    void setVisited(bool a);
    void processAddingObject();
    void addObject(const Object &obj);
};
#endif