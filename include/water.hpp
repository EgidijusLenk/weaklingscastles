#pragma once
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "resource.hpp"

class Water : public Resource
{
private:
public:
    Water(sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now);
    ~Water();
    sf::RectangleShape shape_ ; //TODO should be private probably
    
    void drawResource() override;
    sf::Vector2f get_position() override;
    sf::FloatRect get_float_rect() override;
    void set_shape_center() override;
    sf::Vector2f get_shape_center() override;
};
