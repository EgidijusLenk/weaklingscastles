#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "stlastar.h"
#include <array>
#include <vector>

// using namespace std; // do i need? NO? yes, check pathfinder.cpp to fix

class PathFinder
{
private:
public:
    PathFinder();
    ~PathFinder();
    // static std::array<int, ((MAP_HEIGHT / PATH_CELL_SIZE) * (MAP_WIDTH / PATH_CELL_SIZE))> passable_area_;
    static std::array<int, ((MAP_HEIGHT / CELL_SIZE) * (MAP_WIDTH / CELL_SIZE))> passable_area_;

    static std::vector<sf::Vector2f> findPath(sf::Vector2f start, sf::Vector2f end);
    static int getMap(int x, int y);
    // static unsigned long long int counter_;
};

class VectorNode // Instead of creating new class, I could try to extend sf::Vector2f
{
private:
public:
    VectorNode(int px, int py);
    VectorNode();
    ~VectorNode();
    int x = 0;
    int y = 0;

    bool GetSuccessors(AStarSearch<VectorNode> *astarsearch, VectorNode *parent_node);
    float GoalDistanceEstimate(VectorNode &otherNode);
    bool IsGoal(VectorNode &otherNode);
    float GetCost(VectorNode &otherNode);
    bool IsSameState(VectorNode &otherNode);
    size_t Hash();
    void PrintNodeInfo();
};