#pragma once

#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "economy.hpp"
#include "button.hpp"


class UI
{
protected:
    Economy& eco_;
    sf::RenderWindow& window_;
    sf::RectangleShape footer_ ;
    sf::RectangleShape minimap_ ;
    sf::RectangleShape left_header_ ;
    sf::Font font_;
    sf::Text gold_;
    sf::Text water_;
    sf::Text wood_;
    sf::Text food_;
    sf::Text population_;

    sf::Text warriors_to_train_que_size_;
    sf::Text peasants_to_train_que_size_;

    sf::Text average_gold_production_;
    sf::Text average_water_production_;

    // Buttons
    Button footer_button_1_ ;
    Button footer_button_2_ ;
    sf::Text footer_button_1_text_;
    sf::Text footer_button_2_text_;

    std::vector<Button*> buttons_;

    std::vector<sf::FloatRect> static_dimensions_;

    void drawMainUI();
    void updateUI();
    void collectDimensions();

public:
    UI(sf::RenderWindow& window, Economy& eco);
    ~UI();
    

    friend class GameManager;
};

