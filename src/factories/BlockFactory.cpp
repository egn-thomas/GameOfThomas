#include "BlockFactory.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

std::unique_ptr<Block> BlockFactory::createBlocks(BlockType type, int posx, int posy, sf::RenderWindow &window)
{
    std::cerr << "BlockFactory::createBlocks type=" << static_cast<int>(type) << " pos=("<<posx<<","<<posy<<")" << std::endl;

    switch (type)
    {
    case LEFT_EXIT:
        std::cerr << "  -> creating LeftExitBlock\n";
        return std::make_unique<LeftExitBlock>(posx, posy, window);
    case TOP_EXIT:
        std::cerr << "  -> creating TopExitBlock\n";
        return std::make_unique<TopExitBlock>(posx, posy, window);
    case RIGHT_EXIT:
        std::cerr << "  -> creating RightExitBlock\n";
        return std::make_unique<RightExitBlock>(posx, posy, window);
    case BOTTOM_EXIT:
        std::cerr << "  -> creating BottomExitBlock\n";
        return std::make_unique<BottomExitBlock>(posx, posy, window);

    case LEFT_TOP_EXIT:
        return std::make_unique<LeftTopExitBlock>(posx, posy, window);
    case LEFT_RIGHT_EXIT:
        return std::make_unique<LeftRightExitBlock>(posx, posy, window);
    case LEFT_BOTTOM_EXIT:
        return std::make_unique<LeftBottomExitBlock>(posx, posy, window);
    case TOP_RIGHT_EXIT:
        return std::make_unique<TopRightExitBlock>(posx, posy, window);
    case TOP_BOTTOM_EXIT:
        return std::make_unique<TopBottomExitBlock>(posx, posy, window);
    case RIGHT_BOTTOM_EXIT:
        return std::make_unique<RightBottomExitBlock>(posx, posy, window);

    case LEFT_TOP_RIGHT_EXIT:
        return std::make_unique<LeftTopRightExitBlock>(posx, posy, window);
    case LEFT_TOP_BOTTOM_EXIT:
        return std::make_unique<LeftTopBottomExitBlock>(posx, posy, window);
    case LEFT_RIGHT_BOTTOM_EXIT:
        return std::make_unique<LeftRightBottomExitBlock>(posx, posy, window);
    case TOP_RIGHT_BOTTOM_EXIT:
        return std::make_unique<TopRightBottomExitBlock>(posx, posy, window);

    case LEFT_TOP_RIGHT_BOTTOM_EXIT:
        return std::make_unique<LeftTopRightBottomExitBlock>(posx, posy, window);
        
    default:
        std::cerr << "  -> unknown BlockType, fallback to LeftExitBlock\n";
        // Fallback: return a simple LeftExitBlock to avoid throwing in this factory
        return std::make_unique<LeftExitBlock>(posx, posy, window);
    }
}