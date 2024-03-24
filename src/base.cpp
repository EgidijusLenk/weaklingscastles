#include "base.hpp"
#include "object.hpp"
#include "global.hpp"

Base::Base(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco) : Object(name, team, position, window, time_now, eco)
{
    health_ = BASE_HEALTH;
    can_move_ = false;
    attack_speed_ = BASE_ATTACK_SPEED;
    damage_ = BASE_DAMAGE;
    range_ = BASE_RANGE;
    aggressive_ = true;
    shape_.setSize(BASE_SIZE);
    shape_.setPosition(position);
    if (team == 1)
    {
        shape_.setOutlineColor(sf::Color(27, 145, 7));
        shape_.setFillColor(sf::Color(97, 219, 83, 100));
    }
    else
    {
        shape_.setOutlineColor(sf::Color(201, 8, 8));
        shape_.setFillColor(sf::Color(201, 112, 109, 100));

        health_text_.setFillColor(sf::Color(201, 8, 8));
    }
    std::cout << "Base created" << std::endl;
}

Base::~Base()
{
    std::cout << "Base destoyed" << std::endl;
}
void Base::drawObject() {
    selected_ ? shape_.setOutlineThickness(6.f) : shape_.setOutlineThickness(1.f);
    window_.draw(shape_);
    drawHealth();
}

sf::FloatRect Base::get_float_rect() {
    return shape_.getGlobalBounds();
};

void Base::set_shape_center(){
    shape_center_ = getObjectCenter(get_float_rect());
};

sf::Vector2f Base::get_shape_center() {
    return shape_center_;
};

void Base::drawTargetline() {
//TODO ? or move to Object
};

sf::Vector2f Base::get_position() {
    return shape_.getPosition();
};
void Base::set_position(sf::Vector2f position) {
    shape_.setPosition(position);
};
