#pragma once
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include <queue>
#include <thread>
#include <chrono>

class Economy
{
private:
    float gold_ = GOLD;
    float water_ = WATER;
    float wood_ = WOOD;
    float food_ = FOOD;
    float population_ = 0.f;


protected:
    // Setters
    void set_gold(float gold);
    void set_water(float water);
    void set_wood(float wood);
    void set_food(float food);
    void set_population(float population);

    // Force decrease resources
    void forceDecreaseGold(float amount);
    void forceDecreaseWater(float amount);
    void forceDecreaseWood(float amount);
    void forceDecreaseFood(float amount);

public:
    Economy();
    ~Economy();
    bool threads_running_ = true;
    std::thread updateThread;
    // Getters
    float get_gold() const;
    float get_water() const;
    float get_wood() const;
    float get_food() const;
    float get_population() const;

    float get_average_gold_production() const;
    float get_average_water_production() const;

    // Increase resources
    void increaseGold(float amount);
    void increaseWater(float amount);
    void increaseWood(float amount);
    void increaseFood(float amount);

    // Decrease resources
    bool decreaseGold(float amount);
    bool decreaseWater(float amount);
    bool decreaseWood(float amount);
    bool decreaseFood(float amount);

    // Other
    unsigned int warriors_to_train_que_size_ = 0;
    unsigned int peasants_to_train_que_size_ = 0;

    // Calculate production
    void updateResourceCount(float goldCount, std::deque<float>& queue);
    void calculateAverageProductionRate(float resource_count, std::deque<float>& queue, float &average_resource_production);
    std::deque<float> gold_production_queue_;
    std::deque<float> water_production_queue_;
    const unsigned int maxQueueSize = 120;
    const unsigned int resource_trend_check_time_ = 2000;


    float average_gold_production_ = 0.f;
    float average_water_production_ = 0.f;

    std::chrono::steady_clock::time_point previous_time_; //TODO I can pass this from main.cpp, #tech_debt++ partly done in peasant resource collection (should copy for atacking) ()NOT WORKING
    std::chrono::steady_clock::time_point last_production_average_time_;


    void depositResources(ResourceType resourceType, float amount);

    void update();
    friend class GameManager;
};

