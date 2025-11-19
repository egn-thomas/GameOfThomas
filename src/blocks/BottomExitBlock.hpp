#ifndef BLOCK_BOTTOM_EXIT_HPP
#define BLOCK_BOTTOM_EXIT_HPP

#include "ExitBlock.hpp"

class BottomExitBlock : public ExitBlock {
public:
    BottomExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(BOTTOM_EXIT, posx, posy, window) {}
};

#endif
