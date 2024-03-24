#pragma once
#include <random>
#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>

#include "egilogger.h"

// General
constexpr unsigned int CELL_SIZE = 16; // 80 x 45
constexpr unsigned int MAP_HEIGHT = 720;
constexpr unsigned int MAP_WIDTH = 1280;
constexpr unsigned int FRAME_DURATION = 16667;
constexpr unsigned int SCREEN_RESIZE = 1; //Not using?

enum Cell
{
    Empty,
    Wall
};

// Initial stats
constexpr float GOLD = 2000.f;
constexpr float WATER = 300.f;
constexpr float WOOD = 1500.f;
constexpr float FOOD = 1500.f;
constexpr float WATER_CONSUMPTION = 0.7f;               // per person
constexpr unsigned int WATER_CONSUMPTION_PERIOD = 5000; // ms
constexpr unsigned int KILL_OBJECT_PERIOD = 60000; // ms


// UI
constexpr float FOOTER_WIDTH = 250.f;
constexpr float FOOTER_HEIGHT = 100.f;
// constexpr float MINIMAP_WIDTH = 250.f;
// constexpr float MINIMAP_HEIGHT = 250.f;
constexpr float LEFT_HEADER_WIDTH = 500.f;
constexpr float LEFT_HEADER_HEIGHT = 30.f;
constexpr float UI_THICKNESS = 3.f;
const sf::Vector2f UI_OFFSET(6.f, 6.f);
constexpr float FOOTER_BUTTON_WIDTH = 30.f;
constexpr float FOOTER_BUTTON_HEIGHT = 30.f;


// Colors
const sf::Color GOLD_COLOR(247, 180, 72);
const sf::Color WATER_COLOR(38, 150, 224);
const sf::Color WOOD_COLOR(122, 98, 54);
const sf::Color FOOD_COLOR(48, 99, 32);

const sf::Color FOOTER_BUTTON_COLOR(24, 77, 21);

// Button actions
enum class ButtonAction{
    CreateWarrior,
    CreatePeasant,
    CreateArcher,
    Null,
};

//Objects
enum class ObjectType {
    Warrior,
    Peasant,
    Base,
};

constexpr float WARRIOR_HEALTH = 100.f;
constexpr float WARRIOR_DAMAGE = 10.f;
constexpr float WARRIOR_RANGE = 7.f;
constexpr float WARRIOR_SPEED = 0.65f;
constexpr float WARRIOR_COST = 100.f;
constexpr unsigned int WARRIOR_ATTACK_SPEED = 1000; //ms
constexpr unsigned int WARRIOR_PRODUTION_TIME = 10000; // ms

constexpr float PEASANT_HEALTH = 100.f;
constexpr float PEASANT_DAMAGE = 3.f;
constexpr float PEASANT_RANGE = 7.f;
constexpr float PEASANT_SPEED = 0.6f;
constexpr float PEASANT_COST = 60.f;
constexpr float PEASANT_INVENTORY_SIZE = 60.f;                   // TODO working peasant could be raided and if killed with resources, killer would get upgrade - nice, never gonna implenet :))
constexpr unsigned int PEASANT_RESOURCE_COLLECTION_SPEED = 1000; //ms
constexpr unsigned int PEASANT_ATTACK_SPEED = 1000; //ms
constexpr unsigned int PEASANT_PRODUTION_TIME = 10000; // ms

constexpr float BASE_HEALTH = 3000.f;
constexpr float BASE_DAMAGE = 20.f;
constexpr float BASE_RANGE = 30.f;
constexpr unsigned int BASE_ATTACK_SPEED = 1000; //ms
const sf::Vector2f BASE_POSITION = {20.f, 50.f};
const sf::Vector2f BASE_SIZE = {100.f, 100.f};
const sf::Vector2f RALLY_POINT = {100.f, 160.f};
const sf::Vector2f RALLY_POINT_2 = {1120.f, 620.f};

const sf::Vector2f BASE_POSITION_TEAM_2 = {MAP_WIDTH - BASE_SIZE.x - 20.f, MAP_HEIGHT - BASE_SIZE.y - 50.f};

sf::Vector2f getObjectCenter(const sf::FloatRect &float_rect);
bool isRadiusCollision(const sf::Vector2f& circle_center, float circle_cadius, const sf::Vector2f& point);

//Resources
enum class ResourceType
{
    Gold,
    Wood,
    Food,
    Water, // TODO could dig canals to base, so it wont starve, enemy could break them
    Null,
};
constexpr float RESOURCE_COLLECTION_RATE = 5.f; // per second per labourer

constexpr float GOLD_QUANTITY = 15000.f;
constexpr bool GOLD_CAN_REGENERATE = false;
constexpr float GOLD_REGENERATION_SPEED = 0.f;

constexpr float WATER_QUANTITY = 20000.f;
constexpr bool WATER_CAN_REGENERATE = true;
constexpr float WATER_REGENERATION_SPEED = 0.1f; // TODO most probably not implemented