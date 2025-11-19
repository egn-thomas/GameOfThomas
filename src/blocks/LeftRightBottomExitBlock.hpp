#ifndef BLOCK_LEFT_RIGHT_BOTTOM_EXIT_HPP
#define BLOCK_LEFT_RIGHT_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class LeftRightBottomExitBlock : public ExitBlock {
public:
    LeftRightBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | RIGHT_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
