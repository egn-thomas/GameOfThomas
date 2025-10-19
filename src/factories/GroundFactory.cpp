#include "GroundFactory.hpp"

std::vector<Ground> GroundFactory::createDefaultGrounds(const sf::Vector2u &windowSize)
{
    std::vector<Ground> grounds;

    grounds.emplace_back(0.f, windowSize.y - 50.f, windowSize.x, 50.f);
    grounds.emplace_back(50.f, 900.f, 500.f, 10.f);
    grounds.emplace_back(600.f, 750.f, 500.f, 50.f);
    grounds.emplace_back(100.f, 600.f, 500.f, 50.f);
    grounds.emplace_back(600.f, 450.f, 500.f, 10.f);
    grounds.emplace_back(100.f, 300.f, 500.f, 10.f);
    grounds.emplace_back(1300.f, 900.f, 100.f, 250.f);
    grounds.emplace_back(0.f, 0.f, 50.f, windowSize.y);
    grounds.emplace_back(windowSize.x - 50.f, 0.f, 50.f, windowSize.y);
    grounds.emplace_back(0.f, 0.f, windowSize.x, 50.f);

    return grounds;
}
