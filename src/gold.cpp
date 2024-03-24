#include "gold.hpp"
#include "resource.hpp"
#include "global.hpp"


Gold::Gold(sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now) : Resource(position, window, time_now)
{
    quantity_ = GOLD_QUANTITY;
    can_regenerate_ = GOLD_CAN_REGENERATE;
    regeneration_speed_ = GOLD_REGENERATION_SPEED;
    resource_type_ = ResourceType::Gold;
    shape_.setSize(sf::Vector2f(50.f, 50.f));
    shape_.setPosition(position);
    shape_.setOutlineColor(sf::Color(148, 92, 3));
    shape_.setFillColor(sf::Color(GOLD_COLOR));
    set_shape_center();
}

Gold::~Gold()
{
}


void Gold::drawResource(){
    selected_ ? shape_.setOutlineThickness(3.f) : shape_.setOutlineThickness(1.f);
    window_.draw(shape_);
    drawQuantity();    
};

sf::Vector2f Gold::get_position(){
    return shape_.getPosition();
};

sf::FloatRect Gold::get_float_rect() {
    return shape_.getGlobalBounds();
};

void Gold::set_shape_center(){
    shape_center_ = getObjectCenter(get_float_rect());
};

sf::Vector2f Gold::get_shape_center() {
    return shape_center_;
};