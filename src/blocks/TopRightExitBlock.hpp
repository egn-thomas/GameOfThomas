#ifndef BLOCK_TOP_RIGHT_EXIT_HPP
#define BLOCK_TOP_RIGHT_EXIT_HPP

#include "ExitBlock.hpp"

class TopRightExitBlock : public ExitBlock {
public:
    TopRightExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(static_cast<BlockType>(TOP_EXIT | RIGHT_EXIT), posx, posy, window) {}
};

#endif
