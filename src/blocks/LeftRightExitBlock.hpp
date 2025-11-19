#ifndef BLOCK_LEFT_RIGHT_EXIT_HPP
#define BLOCK_LEFT_RIGHT_EXIT_HPP

#include "ExitBlock.hpp"

class LeftRightExitBlock : public ExitBlock {
public:
    LeftRightExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | RIGHT_EXIT), posx, posy, window) {}
};

#endif
