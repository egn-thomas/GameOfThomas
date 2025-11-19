#ifndef BLOCK_LEFT_TOP_RIGHT_EXIT_HPP
#define BLOCK_LEFT_TOP_RIGHT_EXIT_HPP

#include "ExitBlock.hpp"

class LeftTopRightExitBlock : public ExitBlock {
public:
    LeftTopRightExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | TOP_EXIT | RIGHT_EXIT), posx, posy, window) {}
};

#endif
