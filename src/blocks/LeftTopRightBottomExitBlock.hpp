#ifndef BLOCK_LEFT_TOP_RIGHT_BOTTOM_EXIT_HPP
#define BLOCK_LEFT_TOP_RIGHT_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class LeftTopRightBottomExitBlock : public ExitBlock {
public:
    LeftTopRightBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | TOP_EXIT | RIGHT_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
