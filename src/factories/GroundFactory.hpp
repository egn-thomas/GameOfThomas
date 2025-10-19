#pragma once
#include "../environnement/Ground.hpp"
#include <vector>

class GroundFactory
{
public:
    static std::vector<Ground> createDefaultGrounds(const sf::Vector2u &windowSize);
};
