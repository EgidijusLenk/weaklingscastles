#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"
#include <array>

class Map
{
private:
public:
    Map(sf::RenderWindow &window);
    ~Map();
    sf::RenderWindow &window_;
    std::array<std::array<Cell, (MAP_HEIGHT / CELL_SIZE)>, (MAP_WIDTH / CELL_SIZE)> map_{}; // 80 x 45
    std::array<int, ((MAP_HEIGHT / CELL_SIZE) * (MAP_WIDTH / CELL_SIZE))> passable_area_;
    std::array<std::string, (MAP_HEIGHT / CELL_SIZE)> map_sketch_ = {
        "################################################################################",
        "################################...............................................#",
        "################################.........#....................###..............#",
        "#........................................#.........###.........##..............#",
        "#........................................#...........#..........##.............#",
        "#..............................................................................#",
        "#..............................................................................#",
        "#...........................................................###................#",
        "#...........................................................###................#",
        "#...........................................................###................#", // 10
        "#..............................................................................#",
        "#..............................................................................#",
        "#..............................................................................#",
        "#...............................###............................................#",
        "#...............................#.#...........###................##............#",
        "#...............................###..............................##............#",
        "#..............................................................................#",
        "#..............................................................................#",
        "#.....................#........................#.........##....................#",
        "#....................##........................#..........#...........##########", // 20
        "#...............................################.....................##........#",
        "#...........##.................................#....................##.........#",
        "#..........##...................#..............#...................##..........#",
        "#.........##....................#.................................##...........#",
        "#........##.....................################...............................#",
        "##########......................#..............................................#",
        "#...............................#..............................................#",
        "#................................................###...........................#",
        "#..................###.............................#...........................#",
        "#..................###.............................#...........................#", // 30
        "#..............................................................................#",
        "#........................##....................................................#",
        "#.........................#.........###........................................#",
        "#####################...............#.#........................................#",
        "#...................#...............###........................................#",
        "#...................#..........................................................#",
        "#...................#..........................................................#",
        "#...................#..........................................................#",
        "#...................#################..........................................#",
        "#...................................#..........................................#", // 40
        "#...................................#..........................................#",
        "#...................................#..........................................#",
        "#...................................#..........................................#",
        "#...................................#..........................................#",
        "################################################################################", // 45
    };

    void convertSketch();
    void convertPassableArea();
    void drawMap();
};
