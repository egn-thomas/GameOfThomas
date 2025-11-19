#ifndef BLOCK_RIGHT_BOTTOM_EXIT_HPP
#define BLOCK_RIGHT_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class RightBottomExitBlock : public ExitBlock {
public:
    RightBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(RIGHT_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
