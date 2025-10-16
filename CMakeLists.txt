cmake_minimum_required(VERSION 3.25)
project(GameOfThomas LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

add_executable(main
    src/main.cpp
    src/characters/GameCharacter.cpp
    src/characters/Player.cpp
    src/characters/NonPlayer.cpp
    src/events/EventManager.cpp
    src/events/MoveEvent.cpp
    src/environnement/Ground.cpp
    src/DevMode.cpp
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)