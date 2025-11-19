#ifndef BLOCK_LEFT_TOP_EXIT_HPP
#define BLOCK_LEFT_TOP_EXIT_HPP

#include "ExitBlock.hpp"

class LeftTopExitBlock : public ExitBlock {
public:
    LeftTopExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | TOP_EXIT), posx, posy, window) {}
};

#endif
