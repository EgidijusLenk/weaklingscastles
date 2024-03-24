#include "resource.hpp"

Resource::Resource(sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point &time_now) : window_(window), position_(position), time_now_(time_now)
{
    font_.loadFromFile("resources/Arial.ttf");
    quantity_text_.setFont(font_);
    quantity_text_.setCharacterSize(16);
    quantity_text_.setFillColor(sf::Color(0, 0, 0));
    quantity_text_.setPosition(sf::Vector2f(position_));

}

Resource::~Resource()
{
}

void Resource::set_quantity(float quantity){
    quantity_ = quantity; //TODO I am setting quantity_ to quantity_ in drawQuantity, also fix this for Object :) #tech_debt++
    // quantity_text_.setString(std::to_string(static_cast<int>(quantity_)));
    if (selected_)
    {
        quantity_text_.setString(std::to_string(static_cast<int>(quantity_)));
    }
}
void Resource::set_selected(bool selected){
    selected_ = selected;
};

void Resource::reduceQuantity(float amount){

    set_quantity(quantity_ - amount);
};

void Resource::drawQuantity(){
    if (selected_)
    {
        quantity_text_.setPosition(sf::Vector2f(position_));
        set_quantity(quantity_); //TODO remove line, thats just to display various info for debug. But it wont refresh if I dont set it here
        window_.draw(quantity_text_);
    }
};
