#ifndef BLOCK_RIGHT_EXIT_HPP
#define BLOCK_RIGHT_EXIT_HPP

#include "ExitBlock.hpp"

class RightExitBlock : public ExitBlock {
public:
    RightExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(RIGHT_EXIT, posx, posy, window) {}
};

#endif
