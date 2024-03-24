#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "object.hpp"
#include "base.hpp"
#include "warrior.hpp"
#include "peasant.hpp"
#include "economy.hpp"

class ObjectFactory
{
private:
    Economy& eco_;
    sf::RenderWindow& window_;
    std::mt19937 gen_;
    std::chrono::steady_clock::time_point &time_now_;
public:
    ObjectFactory(sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco);
    ~ObjectFactory();
    std::shared_ptr<Object> createObject(const ObjectType &object_type, const int team, const sf::Vector2f &position);
};

