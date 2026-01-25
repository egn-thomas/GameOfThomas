#ifndef NODE_HPP
#define NODE_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>

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

    Node(int id, int xPos, int yPos);
    void addNeighbors(Node *n);
    bool isPositionsNeighbors(Node *n);
    std::vector<Node *> getNeighbors();
    int getxPos() { return xPos; }
    int getyPos() { return yPos; }
    void removeWallWith(Node *other);
    bool isVisited();
    void setVisited(bool a);
};
#endif