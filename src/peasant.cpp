#include "peasant.hpp"
#include "object.hpp"
#include "global.hpp"

// No idea what was the following TODO remove? oh its closestPointOnRect
// sf::Vector2f closestPointOnRect(const sf::Vector2f& point, const sf::FloatRect& rect) {
//     // Calculate the difference between the point and the rectangle's position
//     float dx = point.x - std::max(rect.left, std::min(point.x, rect.left + rect.width));
//     float dy = point.y - std::max(rect.top, std::min(point.y, rect.top + rect.height));

//     // If the point is inside the rectangle, it's already the closest point
//     if (dx == 0 && dy == 0) {
//         return point;
//     }

//     // Calculate the closest point on the rectangle's boundary
//     float closestX = std::max(rect.left, std::min(point.x, rect.left + rect.width));
//     float closestY = std::max(rect.top, std::min(point.y, rect.top + rect.height));

//     return sf::Vector2f(closestX, closestY);
// }
//------


Peasant::Peasant(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco) : Object(name, team, position, window, time_now, eco)
{  
    health_ = PEASANT_HEALTH;
    speed_ = PEASANT_SPEED;
    attack_speed_ = PEASANT_ATTACK_SPEED;
    damage_ = PEASANT_DAMAGE;
    range_ = PEASANT_RANGE;
    labourer_ = true;
    aggressive_ = false;
    shape_.setRadius(8.f);
    shape_.setPointCount(12);
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
        nearest_resource_depot_ = {BASE_POSITION_TEAM_2.x + 40.f, BASE_POSITION_TEAM_2.y + 40.f};
    }
    // std::cout << "Peasant created" << std::endl;
}

Peasant::~Peasant()
{
    std::cout << "Peasant destoyed" << std::endl;
}
void Peasant::drawObject() {
    selected_ ? shape_.setOutlineThickness(2.f) : shape_.setOutlineThickness(0.5f);
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

sf::FloatRect Peasant::get_float_rect() {
    return shape_.getGlobalBounds();
};

void Peasant::set_shape_center(){
    shape_center_ = getObjectCenter(get_float_rect());
};

sf::Vector2f Peasant::get_shape_center() {
    return shape_center_;
};



void Peasant::drawTargetline() {
    sf::Vertex target_line_[] = {
        sf::Vertex(sf::Vector2f(10.f, 10.f)),
        sf::Vertex(sf::Vector2f(150.f, 150.f))
    };
};

sf::Vector2f Peasant::get_position() {
    return shape_.getPosition();
};
void Peasant::set_position(sf::Vector2f position) {
    shape_.setPosition(position);
};

void Peasant::set_in_collection_cycle(bool flag){
        in_collection_cycle_ = flag;
};

bool Peasant::is_in_collection_cycle(){
        return in_collection_cycle_;
};

void Peasant::collectResource(std::shared_ptr<Resource> target_resource){

    // std::cout << name_ << " peasant, go to work " << std::endl;
    target_resource_ = target_resource;
    sf::Vector2f temp_target = target_resource_->get_shape_center();
    setTarget(temp_target);
    set_in_collection_cycle(true);

};

void Peasant::updateObjectSpecifics()
{
    // Peasant is working
    if (in_collection_cycle_)
    {
            // std::cout << " peasant distance_to_target_.x " << distance_to_target_.x << " " << distance_to_target_.y  <<  std::endl;
        sf::Vector2f temp_target = target_resource_->get_shape_center();
        bool far_from_destination = (std::abs(distance_to_target_.x) > range_ | std::abs(distance_to_target_.y) > range_);
        if (target_center_ == nearest_resource_depot_ || target_center_ == temp_target)
        {
            if (inventory_full_ && !far_from_destination && target_center_ == nearest_resource_depot_)
            {
                eco_.depositResources(resource_type_, resource_cout_);
                resource_cout_ = 0.f;
                inventory_full_ = false;
                setTarget(temp_target);
                // std::cout << " peasant emptied inventory " << std::endl;
            }
            else if (!(target_resource_->depleted_) && (!far_from_destination | get_float_rect().intersects(target_resource_->get_float_rect())) && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_attack_time_).count() >= resource_collection_speed_)
            {
                // get_float_rect().intersects(target_resource_->get_float_rect())
                if (resource_type_ != target_resource_->resource_type_)
                {
                    // std::cout << " peasants resource type changed " << std::endl;
                    if (resource_cout_ != 0.f)
                    {
                        inventory_full_ = true;
                    }
                }
                if (inventory_full_)
                {
                    // std::cout << " peasant got some resources to deposit first " << std::endl;
                    setTarget(nearest_resource_depot_);
                    set_arrived(false);
                }
                else
                {
                    resource_type_ = target_resource_->resource_type_;
                    last_attack_time_ = std::chrono::steady_clock::now();
                    if (add_to_inventory(resource_collection_rate_))
                    {
                    target_resource_->reduceQuantity(resource_collection_rate_);
                    // std::cout << " peasant collecting " << std::endl;
                    } else
                    {
                        // TODO be back to base to return resoruces
                        //  std::cout << " peasants inventory full " << std::endl;
                        inventory_full_ = true;
                        setTarget(nearest_resource_depot_);
                        set_arrived(false);
                    }
                }
            }
            else if (arrived_ && far_from_destination && target_center_ == nearest_resource_depot_)
            {
                in_collection_cycle_ = false; // TODO not working #techdebt++
            }
        }
        else 
        {
            in_collection_cycle_ = false;
        }
    }
};
    bool Peasant::add_to_inventory(float amount){
        if (resource_cout_ + amount > inventory_size_){
            return false;
        }
        else
        {
            resource_cout_ += amount;
        }
        return true; 
    };