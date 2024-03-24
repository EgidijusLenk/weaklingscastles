#include "warrior.hpp"
#include "object.hpp"
#include "global.hpp"

Warrior::Warrior(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now, Economy& eco) : Object(name, team, position, window, time_now, eco)
{  
    health_ = WARRIOR_HEALTH;
    speed_ = WARRIOR_SPEED;
    attack_speed_ = WARRIOR_ATTACK_SPEED;
    damage_ = WARRIOR_DAMAGE;
    range_ = WARRIOR_RANGE;
    shape_.setSize(sf::Vector2f(15.f, 15.f));
    shape_.setPosition(position);
    if (team == 1)
    {
        shape_.setOutlineColor(sf::Color(27, 145, 7));
        shape_.setFillColor(sf::Color(97, 219, 83, 100));
    }
    else
    {
        shape_.setOutlineColor(sf::Color(201, 8, 8));
        shape_.setFillColor(sf::Color(201, 112, 109, 200));

        health_text_.setFillColor(sf::Color(201, 8, 8));
    }
    // std::cout << "Warrior created" << std::endl;
}

Warrior::~Warrior()
{
    std::cout << "Warrior destoyed" << std::endl;
}
void Warrior::drawObject() {
    selected_ ? shape_.setOutlineThickness(2.f) : shape_.setOutlineThickness(.5f);
    window_.draw(shape_);
    if (selected_)
    {   
        drawHealth();
        sf::Vertex line[] =
        {
            sf::Vertex(shape_center_),
            sf::Vertex(target_center_)
        };

        window_.draw(line, 2, sf::Lines);
    }
}

sf::FloatRect Warrior::get_float_rect() {
    return shape_.getGlobalBounds();
};

void Warrior::set_shape_center(){
    shape_center_ = getObjectCenter(get_float_rect());
};

sf::Vector2f Warrior::get_shape_center() {
    return shape_center_;
};



void Warrior::drawTargetline() {
    sf::Vertex target_line_[] = {
        sf::Vertex(sf::Vector2f(10.f, 10.f)),
        sf::Vertex(sf::Vector2f(150.f, 150.f))
    };
};

sf::Vector2f Warrior::get_position() {
    return shape_.getPosition();
};
void Warrior::set_position(sf::Vector2f position) {
    shape_.setPosition(position);
};
