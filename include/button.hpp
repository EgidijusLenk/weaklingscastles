#pragma once

#include "global.hpp"
#include <SFML/Graphics.hpp>

class Button : public sf::RectangleShape
{
private:
    
public:
    Button();
    ~Button();

    ButtonAction action_ = ButtonAction::Null;
};

// void click();