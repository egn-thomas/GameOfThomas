#ifndef BLOCK_LEFT_TOP_BOTTOM_EXIT_HPP
#define BLOCK_LEFT_TOP_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class LeftTopBottomExitBlock : public ExitBlock {
public:
    LeftTopBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | TOP_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
