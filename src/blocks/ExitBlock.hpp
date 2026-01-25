#ifndef EXIT_BLOCK_HPP
#define EXIT_BLOCK_HPP

#include "Block.hpp"
#include "BlockEnum.hpp"
#include "../environnement/TexturedGround.hpp"
#include "../environnement/LadderGround.hpp"
#include <vector>
#include <memory>

class ExitBlock : public Block {
public:
    ExitBlock(BlockType type, int posx, int posy, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window, int x, int y, int cellSize) override;
    std::vector<std::unique_ptr<Ground>> createGrounds() override;
    std::vector<std::unique_ptr<Object>> createObjects() override;
    std::vector<std::unique_ptr<GameCharacter>> createCharacters() override;

protected:
    // construit les grounds en fonction du mask
    std::vector<std::unique_ptr<Ground>> buildGrounds(BlockType mask, int posx, int posy, sf::RenderWindow& window);

    BlockType mask_;
    int posx_;
    int posy_;
    std::vector<std::unique_ptr<Ground>> grounds_;
    sf::RenderWindow* window_;
    static constexpr int GRID_COLS = 7;
    static constexpr int GRID_ROWS = 4;
    static constexpr float CHEST_SPAWN_CHANCE = 20.f;  // 20% de chance
    static constexpr float CANDLE_SPAWN_CHANCE = 30.f;    // 30% de chance
};

#endif
