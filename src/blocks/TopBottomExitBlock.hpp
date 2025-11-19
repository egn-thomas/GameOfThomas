#ifndef BLOCK_TOP_BOTTOM_EXIT_HPP
#define BLOCK_TOP_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class TopBottomExitBlock : public ExitBlock {
public:
    TopBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(TOP_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
