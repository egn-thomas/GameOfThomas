#pragma once
#include "../environnement/Ground.hpp"
#include <vector>
#include <memory>

class GroundFactory
{
public:
    static std::vector<std::unique_ptr<Ground>> createDefaultGrounds(const sf::Vector2u &windowSize);
};
