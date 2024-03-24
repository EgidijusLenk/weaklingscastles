#pragma once

#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "resource.hpp"
#include "global.hpp"
#include "objectfactory.hpp"
#include "resourcefactory.hpp"
#include "ui.hpp"
#include "economy.hpp"
#include "map.hpp"
#include "pathfinder.hpp"

class GameManager
{
private:
    ObjectFactory of_;
    ResourceFactory rf_;
    UI ui_;
    sf::RectangleShape selection_box_;
    sf::Vector2f selection_start_;
    sf::Vector2f selection_end_;
    sf::FloatRect selection_flaot_rect_;
    sf::RenderWindow& window_;
    bool selecting_ = false;
    int team_;
    unsigned int water_consumption_period = WATER_CONSUMPTION_PERIOD;
    std::vector<std::shared_ptr<Object>> selected_objects_ = {};
    std::vector<std::shared_ptr<Object>> buildings_ = {};
    std::vector<std::shared_ptr<Resource>> selected_resources_ = {};

    std::chrono::steady_clock::time_point previous_time_;
    std::chrono::steady_clock::time_point last_warrior_trained_;
    std::chrono::steady_clock::time_point last_peasant_trained_;
    std::chrono::steady_clock::time_point last_comsumed_water_;
    std::chrono::steady_clock::time_point last_scenario_run_;
    std::chrono::steady_clock::time_point last_scenario_water_raid_;
    std::chrono::steady_clock::time_point last_scenario_gold_raid_;
    std::chrono::steady_clock::time_point last_scenario_base_raid_;

public:
    std::queue<std::shared_ptr<Object>> warriors_to_train_ = {};
    std::queue<std::shared_ptr<Object>> peasants_to_train_ = {};
    std::vector<std::shared_ptr<Object>> objects_ = {};
    std::vector<std::shared_ptr<Object>> *enemy_objects_ptr_ = nullptr;
    std::vector<std::shared_ptr<Resource>> resources_ = {};
    Economy eco_;
    Map map_;
    std::chrono::steady_clock::time_point time_now_;
    std::mt19937 gen_;
    GameManager(sf::RenderWindow &window, std::chrono::steady_clock::time_point time_now, int team);
    ~GameManager();
    void drawMap();
    void updateCurrentTime();
    void startSelecting();
    bool selecting();
    void keepSelecting();
    void endSelection();
    sf::Vector2f getMousePosition();
    void drawSelection();
    void manageSelection(sf::Event& event);
    void createObject(const ObjectType &object_type, const int team, const sf::Vector2f &position);
    void createResource(const ResourceType &resource_type, const sf::Vector2f &position);
    void drawObjects();
    void drawResources();
    void collectSelectedObjects();
    void setTarget(sf::Event& event);
    void update();
    bool checkSelectionCollision(const std::shared_ptr<Object>& object);
    bool checkSelectionCollision(const std::shared_ptr<Resource>& resource);
    void drawUI();
    void handleButtonAction(ButtonAction action);
    std::shared_ptr<Object> queObject(const ObjectType &object_type, const int team, const sf::Vector2f &position);
    void trainWarrior();
    void trainPeasant();
    void consumeWater();
    void botScenario();
};
