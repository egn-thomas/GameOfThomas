#ifndef BLOCK_FACTORY_HPP
#define BLOCK_FACTORY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../blocks/BlockEnum.hpp"
#include "../blocks/LeftExitBlock.hpp"
#include "../blocks/TopExitBlock.hpp"
#include "../blocks/RightExitBlock.hpp"
#include "../blocks/BottomExitBlock.hpp"

#include "../blocks/LeftTopExitBlock.hpp"
#include "../blocks/LeftRightExitBlock.hpp"
#include "../blocks/LeftBottomExitBlock.hpp"
#include "../blocks/TopRightExitBlock.hpp"
#include "../blocks/TopBottomExitBlock.hpp"
#include "../blocks/RightBottomExitBlock.hpp"

#include "../blocks/LeftTopRightExitBlock.hpp"
#include "../blocks/LeftTopBottomExitBlock.hpp"
#include "../blocks/LeftRightBottomExitBlock.hpp"
#include "../blocks/TopRightBottomExitBlock.hpp"

#include "../blocks/LeftTopRightBottomExitBlock.hpp"

class BlockFactory {
public:
    static std::unique_ptr<Block> createBlocks(BlockType type, int posx, int posy, sf::RenderWindow& window);
};

#endif