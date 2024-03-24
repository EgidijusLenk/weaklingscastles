#include "water.hpp"
#include "resource.hpp"
#include "global.hpp"


Water::Water(sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now) : Resource(position, window, time_now)
{
    quantity_ = WATER_QUANTITY;
    can_regenerate_ = WATER_CAN_REGENERATE;
    resource_type_ = ResourceType::Water;
    regeneration_speed_ = WATER_REGENERATION_SPEED;
    shape_.setSize(sf::Vector2f(50.f, 50.f));
    shape_.setPosition(position);
    shape_.setOutlineColor(sf::Color(7, 89, 166));
    shape_.setFillColor(sf::Color(WATER_COLOR));
    set_shape_center();
}

Water::~Water()
{
}


void Water::drawResource(){
    selected_ ? shape_.setOutlineThickness(3.f) : shape_.setOutlineThickness(1.f);
    window_.draw(shape_);
    drawQuantity();    
};

sf::Vector2f Water::get_position(){
    return shape_.getPosition();
};

sf::FloatRect Water::get_float_rect() {
    return shape_.getGlobalBounds();
};

void Water::set_shape_center(){
    shape_center_ = getObjectCenter(get_float_rect());
};

sf::Vector2f Water::get_shape_center() {
    return shape_center_;
};