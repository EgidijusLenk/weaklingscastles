
#include <SFML/Graphics.hpp>
#include "resourcefactory.hpp"

ResourceFactory::ResourceFactory(sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now) : window_(window), time_now_(time_now)
{
}

ResourceFactory::~ResourceFactory()
{
    std::cout << "ResourceFactory destoyed" << std::endl;
}
std::shared_ptr<Resource> ResourceFactory::createResource(const ResourceType &resource_type, const sf::Vector2f &position)
{   

    switch (resource_type)
    {
    case ResourceType::Gold:
        return std::make_shared<Gold>(position, window_, time_now_);
        break;
        
    // case ResourceType::Wood:
    //     return std::make_shared<Wood>(position, window_, time_now_);
    //     break;
    
    // case ResourceType::Food:
    //     return std::make_shared<Food>(position, window_, time_now_);
    //     break;
    
    case ResourceType::Water:
        return std::make_shared<Water>(position, window_, time_now_);
        break;
    
    default:
        throw std::runtime_error("Error creating Resource: no such resource type exists");
        break;
    }
}
