#pragma once
#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "global.hpp"

class Warrior : public Object
{
private:
    
public:
    sf::RectangleShape shape_ ; //TODO should be private probably
    Warrior(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco);
    ~Warrior();
    void drawObject() override;
    sf::FloatRect get_float_rect() override;
    sf::Vector2f get_shape_center() override;
    void set_shape_center() override;
    void drawTargetline() override;
    sf::Vector2f get_position() override;
    void set_position(sf::Vector2f position) override;

};


