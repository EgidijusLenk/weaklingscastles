#pragma once
#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "global.hpp"
#include "resource.hpp"

class Peasant : public Object
{
private:

public:
    sf::CircleShape shape_;
    float resource_collection_rate_ = RESOURCE_COLLECTION_RATE;
    unsigned int resource_collection_speed_ = PEASANT_RESOURCE_COLLECTION_SPEED;
    std::shared_ptr<Resource> target_resource_;
    sf::Vector2f nearest_resource_depot_ = {BASE_POSITION.x + 40.f, BASE_POSITION.y + 40.f}; // TOOD could get base object ? Yes

    // Resource gathering
    ResourceType resource_type_ = ResourceType::Null;
    float resource_cout_ = 0.f; 
    float inventory_size_ = PEASANT_INVENTORY_SIZE;
    bool inventory_full_ = false;
    bool in_collection_cycle_ = false;

    Peasant(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco);
    ~Peasant();
    void drawObject() override;
    sf::FloatRect get_float_rect() override;
    sf::Vector2f get_shape_center() override;
    void set_shape_center() override;
    void drawTargetline() override;
    sf::Vector2f get_position() override;
    void set_position(sf::Vector2f position) override;
    void set_in_collection_cycle(bool flag);
    bool is_in_collection_cycle();
    void collectResource(std::shared_ptr<Resource> target_resource);
    void updateObjectSpecifics() override;
    bool add_to_inventory(float amount);

};


