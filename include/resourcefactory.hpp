#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "resource.hpp"
#include "gold.hpp"
#include "water.hpp"

class ResourceFactory
{
private:
    sf::RenderWindow& window_;
    std::mt19937 gen_;
    std::chrono::steady_clock::time_point &time_now_;
public:
    ResourceFactory(sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now);
    ~ResourceFactory();
    std::shared_ptr<Resource> createResource(const ResourceType &resource_type, const sf::Vector2f &position);
};

