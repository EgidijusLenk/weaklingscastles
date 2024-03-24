#include "economy.hpp"

Economy::Economy() : updateThread(&Economy::update, this)
{
    gold_production_queue_.push_back(GOLD);
    water_production_queue_.push_back(WATER);
}

Economy::~Economy()
{   
    threads_running_ = false;
    if (updateThread.joinable()) {
        updateThread.join();
        std::cout << "Economy updateThread destroyed." << std::endl;
    }
}

float Economy::get_gold() const { return gold_; }
float Economy::get_water() const { return water_; }
float Economy::get_wood() const { return wood_; }
float Economy::get_food() const { return food_; }
float Economy::get_population() const { return population_; }
float Economy::get_average_gold_production() const { return average_gold_production_; }
float Economy::get_average_water_production() const { return average_water_production_; }

void Economy::set_gold(float gold) { gold_ = gold; }
void Economy::set_water(float water) { water_ = water; }
void Economy::set_wood(float wood) { wood_ = wood; }
void Economy::set_food(float food) { food_ = food; }

void Economy::set_population(float population) { population_ = population; }

void Economy::increaseGold(float amount) {
    gold_ += amount;
}

void Economy::increaseWater(float amount) {
    water_ += amount;
}

void Economy::increaseWood(float amount) {
    wood_ += amount;
}

void Economy::increaseFood(float amount) {
    food_ += amount;
}

bool Economy::decreaseGold(float amount) {
    if (gold_ - amount >= 0.0f) {
        gold_ -= amount;
        return true;
    } else {
        return false;
    }
}

bool Economy::decreaseWater(float amount) {
    if (water_ - amount >= 0.0f) {
        water_ -= amount;
        return true;
    } else {
        return false;
    }
}

bool Economy::decreaseWood(float amount) {
    if (wood_ - amount >= 0.0f) {
        wood_ -= amount;
        return true;
    } else {
        return false;
    }
}

bool Economy::decreaseFood(float amount) {
    if (food_ - amount >= 0.0f) {
        food_ -= amount;
        return true;
    } else {
        return false;
    }
}

void Economy::forceDecreaseGold(float amount) {
    gold_ -= amount;
}

void Economy::forceDecreaseWater(float amount) {
    water_ -= amount;
}

void Economy::forceDecreaseWood(float amount) {
    wood_ -= amount;
}

void Economy::forceDecreaseFood(float amount) {
    food_ -= amount;
}

void Economy::depositResources(ResourceType resourceType, float amount) {
    if (amount < 0.0f) {
        std::cout << "Error: Cannot unload a negative amount of resources." << std::endl;
        return;
    }
    switch (resourceType) {
        case ResourceType::Gold:
            increaseGold(amount);
            break;
        case ResourceType::Water:
            increaseWater(amount);
            break;
        case ResourceType::Wood:
            increaseWood(amount);
            break;
        case ResourceType::Food:
            increaseFood(amount);
            break;
        case ResourceType::Null:
            std::cout << "Error: Cannot unload resources of type Null." << std::endl;
            break;
    }
}

void Economy::updateResourceCount(float resource_count, std::deque<float>& queue) {
 
    if (!queue.empty()) 
    {
        queue.push_back(resource_count);
    // std::cout << " queue.push_back..." << std::endl;
    }

    if (queue.size() > maxQueueSize) {
        queue.pop_front();
    // std::cout << " queue.pop_front..." << std::endl;
    }

    
}

// sita galiu pritaikyt visiems 
void Economy::calculateAverageProductionRate(float resource_count, std::deque<float>& queue, float &average_resource_production){ // Good enough
    if (!queue.empty()) {
        float sum = 0.0f;
        for (float value : queue) {
            sum += value;
        }
        average_resource_production = resource_count - (sum / maxQueueSize);
    }
}

void Economy::update(){
    while (threads_running_)
    {
    // if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_production_average_time_).count() >= resource_trend_check_time_) 
    // {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // std::cout << "thread spin." << std::endl;

    last_production_average_time_ = std::chrono::steady_clock::now();
    updateResourceCount(gold_, gold_production_queue_);
    updateResourceCount(water_, water_production_queue_);
    calculateAverageProductionRate(gold_, gold_production_queue_, average_gold_production_);
    calculateAverageProductionRate(water_, water_production_queue_, average_water_production_);
    // }
    }
};