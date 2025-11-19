#ifndef BLOCK_TOP_EXIT_HPP
#define BLOCK_TOP_EXIT_HPP

#include "ExitBlock.hpp"

class TopExitBlock : public ExitBlock {
public:
    TopExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(TOP_EXIT, posx, posy, window) {}
};

#endif
