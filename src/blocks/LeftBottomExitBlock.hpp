#ifndef BLOCK_LEFT_BOTTOM_EXIT_HPP
#define BLOCK_LEFT_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class LeftBottomExitBlock : public ExitBlock {
public:
    LeftBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(LEFT_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
