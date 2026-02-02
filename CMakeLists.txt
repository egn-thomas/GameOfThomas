cmake_minimum_required(VERSION 3.25)
project(GameOfThomas LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

add_executable(main
    src/main.cpp
    src/blocks/ExitBlock.cpp
    src/characters/GameCharacter.cpp
    src/characters/Player.cpp
    src/characters/NonPlayer.cpp
    src/characters/Candle.cpp
    src/events/EventManager.cpp
    src/events/MoveEvent.cpp
    src/ui/PauseMenu.cpp
    src/ui/UIManager.cpp
    src/ui/UIManagerNotifications.cpp
    src/ui/InventoryMenu.cpp
    src/environnement/ModelGenerator.cpp
    src/environnement/Node.cpp
    src/environnement/Ground.cpp
    src/environnement/TexturedGround.cpp
    src/environnement/LadderGround.cpp
    src/DevMode.cpp
    src/factories/CharacterFactory.cpp
    src/factories/BlockFactory.cpp
    src/objects/Object.cpp
    src/objects/Chest.cpp
    src/objects/Door.cpp
    src/items/HealthPotion.cpp
    src/items/HealthAmulet.cpp
    src/items/DamageAmulet.cpp
    src/items/ManaPotion.cpp
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

# add_executable(debug_maze
#     src/tools/debug_maze.cpp
#     src/environnement/ModelGenerator.cpp
#     src/environnement/Node.cpp
#     src/objects/Object.cpp
#     src/objects/Chest.cpp
#     src/objects/Door.cpp
# )
# target_compile_features(debug_maze PRIVATE cxx_std_17)
# target_link_libraries(debug_maze PRIVATE sfml-graphics sfml-window sfml-system)