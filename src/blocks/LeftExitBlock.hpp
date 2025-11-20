#ifndef BLOCK_LEFT_EXIT_HPP
#define BLOCK_LEFT_EXIT_HPP

#include "ExitBlock.hpp"

class LeftExitBlock : public ExitBlock
{
public:
    LeftExitBlock(int posx, int posy, sf::RenderWindow& window) : ExitBlock(LEFT_EXIT, posx, posy, window) {};
};

#endif
