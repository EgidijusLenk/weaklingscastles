#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"

class Resource
{
private:
public:
    Resource(sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now);
    ~Resource();

    float quantity_ = 10000.f;
    bool depleted_ = false;
    bool can_regenerate_ = false;
    float regeneration_speed_ = 1.f;
    ResourceType resource_type_ = ResourceType::Null;

    std::chrono::steady_clock::time_point &time_now_;
    sf::Text quantity_text_;
    sf::Font font_;
    sf::Vector2f position_ = { 0.f, 0.f};
    sf::Vector2f shape_center_ = { 0.f, 0.f};
    sf::RenderWindow& window_;

    bool selectable_ = true;
    bool selected_ = false;
    bool in_selection_ = false;

    void set_quantity(float quantity);
    void set_selected(bool selected);
    void reduceQuantity(float amount);
    void drawQuantity();


    virtual void drawResource() = 0;
    virtual sf::Vector2f get_position() = 0;
    virtual sf::FloatRect get_float_rect() = 0;
    virtual sf::Vector2f get_shape_center() = 0;
    virtual void set_shape_center() = 0;
};

