#ifndef BLOCK_TOP_RIGHT_BOTTOM_EXIT_HPP
#define BLOCK_TOP_RIGHT_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class TopRightBottomExitBlock : public ExitBlock {
public:
    TopRightBottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(TOP_EXIT | RIGHT_EXIT | BOTTOM_EXIT), posx, posy, window) {}
};

#endif
