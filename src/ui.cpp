#include "ui.hpp"


void prepareText(sf::Text &text, sf::Font &font, int char_size, const sf::Color &fill_color, sf::Vector2f position, float value ){
    text.setFont(font);
    text.setCharacterSize(char_size);
    text.setFillColor(sf::Color(fill_color));
    text.setPosition(position);
    text.setString(std::to_string(static_cast<int>(value)));
};

void prepareText(sf::Text &text, sf::Font &font, int char_size, const sf::Color &fill_color, sf::Vector2f position, std::string value ){
    text.setFont(font);
    text.setCharacterSize(char_size);
    text.setFillColor(sf::Color(fill_color));
    text.setPosition(position);
    text.setString(value);
};

UI::UI(sf::RenderWindow& window, Economy& eco) : window_(window), eco_(eco)
{
    font_.loadFromFile("resources/Arial.ttf");
    
    // Shapes
    minimap_.setSize(sf::Vector2f(MAP_WIDTH / 4, MAP_HEIGHT / 4));
    minimap_.setOutlineColor(sf::Color(30, 31, 36));
    minimap_.setFillColor(sf::Color(40, 41, 46));
    minimap_.setOutlineThickness(UI_THICKNESS); //REMINDER i think thickness act as margin not padding in html
    minimap_.setPosition(0.f, MAP_HEIGHT - UI_THICKNESS - (MAP_HEIGHT / 4) );
    footer_.setSize(sf::Vector2f(FOOTER_WIDTH, FOOTER_HEIGHT));
    footer_.setOutlineColor(sf::Color(30, 31, 36));
    footer_.setFillColor(sf::Color(40, 41, 46));
    footer_.setOutlineThickness(UI_THICKNESS); //REMINDER i think thickness act as margin not padding in html
    footer_.setPosition((MAP_WIDTH / 4) + UI_THICKNESS, MAP_HEIGHT - UI_THICKNESS - FOOTER_HEIGHT );
    left_header_.setSize(sf::Vector2f(LEFT_HEADER_WIDTH, LEFT_HEADER_HEIGHT));
    left_header_.setOutlineColor(sf::Color(30, 31, 36));
    left_header_.setFillColor(sf::Color(40, 41, 46));
    left_header_.setOutlineThickness(UI_THICKNESS); //REMINDER i think thickness act as margin not padding in html
    left_header_.setPosition(UI_THICKNESS,  UI_THICKNESS);

    // Buttons //TODO do func - DRY!
    footer_button_1_.setSize(sf::Vector2f(FOOTER_BUTTON_WIDTH, FOOTER_BUTTON_HEIGHT));
    footer_button_1_.setOutlineColor(sf::Color(30, 31, 36));
    footer_button_1_.setFillColor(sf::Color(FOOTER_BUTTON_COLOR));
    footer_button_1_.setOutlineThickness(UI_THICKNESS); //REMINDER i think thickness act as margin not padding in html
    footer_button_1_.setPosition(footer_.getPosition() + UI_OFFSET);
    footer_button_1_.action_ = ButtonAction::CreateWarrior;
    footer_button_2_.setSize(sf::Vector2f(FOOTER_BUTTON_WIDTH, FOOTER_BUTTON_HEIGHT));
    footer_button_2_.setOutlineColor(sf::Color(30, 31, 36));
    footer_button_2_.setFillColor(sf::Color(FOOTER_BUTTON_COLOR));
    footer_button_2_.setOutlineThickness(UI_THICKNESS); //REMINDER i think thickness act as margin not padding in html
    footer_button_2_.setPosition(footer_button_1_.getPosition().x + FOOTER_BUTTON_WIDTH + (3 * UI_THICKNESS), footer_button_1_.getPosition().y);
    footer_button_2_.action_ = ButtonAction::CreatePeasant;







    collectDimensions();
}

UI::~UI()
{
}

void UI::drawMainUI(){
    window_.draw(minimap_);
    window_.draw(footer_);
    window_.draw(left_header_);
    window_.draw(gold_);
    window_.draw(water_);
    window_.draw(wood_);
    window_.draw(food_);
    window_.draw(population_);
    window_.draw(footer_button_1_);
    window_.draw(footer_button_2_);
    window_.draw(footer_button_1_text_);
    window_.draw(footer_button_2_text_);
    window_.draw(warriors_to_train_que_size_);
    window_.draw(peasants_to_train_que_size_);
    window_.draw(average_gold_production_);
    window_.draw(average_water_production_);

};

void UI::updateUI(){
    prepareText(gold_, font_, 16, GOLD_COLOR, {UI_THICKNESS, UI_THICKNESS}, eco_.get_gold() );
    prepareText(water_, font_, 16, WATER_COLOR, {gold_.getPosition().x + 50.f, UI_THICKNESS}, eco_.get_water() );
    prepareText(wood_, font_, 16, WOOD_COLOR, {water_.getPosition().x + 50.f, UI_THICKNESS}, eco_.get_wood() );
    prepareText(food_, font_, 16, FOOD_COLOR, {wood_.getPosition().x + 50.f, UI_THICKNESS}, eco_.get_food() );
    prepareText(population_, font_, 16, {255,255,255}, {food_.getPosition().x + 50.f, UI_THICKNESS}, eco_.get_population() );

    prepareText(average_gold_production_, font_, 13, GOLD_COLOR, {UI_THICKNESS, 17.f}, eco_.get_average_gold_production() );
    prepareText(average_water_production_, font_, 13, WATER_COLOR, {gold_.getPosition().x + 50.f, 17.f}, eco_.get_average_water_production() );

    prepareText(footer_button_1_text_, font_, 16, GOLD_COLOR, footer_button_1_.getPosition(), "W");
    prepareText(footer_button_2_text_, font_, 16, GOLD_COLOR, footer_button_2_.getPosition(), "P");

    prepareText(warriors_to_train_que_size_, font_, 25, WOOD_COLOR, footer_button_1_.getPosition() + UI_OFFSET, eco_.warriors_to_train_que_size_);
    prepareText(peasants_to_train_que_size_, font_, 25, WOOD_COLOR, footer_button_2_.getPosition() + UI_OFFSET, eco_.peasants_to_train_que_size_);
    
}
void UI::collectDimensions(){
    // Statics
    static_dimensions_.push_back(minimap_.getGlobalBounds());
    static_dimensions_.push_back(footer_.getGlobalBounds());
    static_dimensions_.push_back(left_header_.getGlobalBounds());

    // Buttons
    buttons_.push_back(&footer_button_1_);
    buttons_.push_back(&footer_button_2_);
};

//-------------------
// // example of relative pos setting for sub shapes
// // // Assuming you have two objects, obj1 and obj2
// sf::Sprite obj1;
// sf::Sprite obj2;

// // Set the position of obj1 (the reference object)
// obj1.setPosition(100.f, 100.f);

// // Calculate the relative position for obj2
// sf::Vector2f offset(50.f, 50.f); // This is the offset relative to obj1
// sf::Vector2f relativePosition = obj1.getPosition() + offset;

// // Set the position of obj2 based on the relative position
// obj2.setPosition(relativePosition);
//-------------------