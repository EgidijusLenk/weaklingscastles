#include "object.hpp"
#include "global.hpp"

Object::Object(std::string name, int team, sf::Vector2f position, sf::RenderWindow &window, std::chrono::steady_clock::time_point &time_now, Economy &eco)
    : updateThread(&Object::update, this), name_(name), team_(team), position_(position), window_(window), time_now_(time_now), eco_(eco)
{
    font_.loadFromFile("resources/Arial.ttf");
    health_text_.setFont(font_);
    health_text_.setCharacterSize(16);
    health_text_.setFillColor(sf::Color(255, 255, 255));
    health_text_.setPosition({position_.x, position_.y - 15.f});
    setHealth(100.f);
    previous_time_ = std::chrono::steady_clock::now();
    last_attack_time_ = previous_time_;
    sf::Vector2f base_position_ = BASE_POSITION;
    previous_target_center_ = {0.f, 0.f};
    target_center_ = {20.f, 20.f};
    path_set_ = false;
}

Object::~Object()
{
    threads_running_ = false;
    if (updateThread.joinable())
    {
        updateThread.join();
        std::cout << "Object updateThread destroyed." << std::endl;
    }
}

void Object::setHealth(float health){
    health_ = health;
    health_text_.setString(std::to_string(static_cast<int>(health_)));
    if (selected_)
    {
        // health_text_.setString(std::to_string(static_cast<int>(target_center_.x)) + " " + name_ + " " + std::to_string(static_cast<bool>(arrived_)) + " " + std::to_string(static_cast<bool>(friendly_collision_adjusting_)) + " ");
        health_text_.setString(std::to_string(static_cast<int>(health_)));
    }
}

void Object::drawHealth(){
    health_text_.setPosition({position_.x, position_.y - 15.f});
    setHealth(health_); //TODO remove line, thats just to display various info for debug. But it wont refresh if I dont set it here
    window_.draw(health_text_);
};
void Object::set_selected(bool selected){
    selected_ = selected;
};

void Object::setTarget(std::shared_ptr<Object> &target){
    path_set_ = false;
    target_ = target;
    updateTargetCenter();
    // std::cout << "-- moving to target object: x " << target->get_shape_center().x << " y " << target->get_shape_center().y << std::endl;
};

void Object::setTarget(sf::Vector2f &target){
    path_set_ = false;
    target_center_ = target;
    previous_target_center_ = target_center_;
};
void Object::removeTargetObj(){
    target_ = nullptr;
    attacker_ = nullptr;
};

void Object::updateTargetCenter(){
    if (target_)
    {
        // target_center_ = getObjectCenter(target_->get_float_rect());
        target_center_ = target_->get_shape_center();
    }
    if (previous_target_center_ != target_center_)
    {
        path_set_ = false;
    }
    previous_target_center_ = target_center_;
};

void Object::updateShapeCenter(){
    this->set_shape_center();
};

int Object::get_team(){
    return team_;
};

void Object::set_friendly_collision(bool flag){
    just_adjusted_friendly_collision_ = flag;
};

bool Object::get_friendly_collision(){
    return just_adjusted_friendly_collision_; // TODO rename var
};

void Object::reduceHealth(float amount)
{
    setHealth(health_ - amount);
};

void Object::handleDeath(){ //TODO so far not used?
    if (health_ <= 0)
    {
        std::cout << "ONE DOWN" << std::endl;

    }
    
};

float Object::get_health(){
    return health_;  
};

std::string Object::get_name(){
return name_;
};

void Object::set_arrived(bool flag){
    if (can_move_)
    {
        arrived_ = flag;
    }
};

void Object::set_aggressive(bool flag){
    aggressive_ = flag;
};

void Object::handleAttack()
{

    if (arrived_ && attacker_ && !target_ && !labourer_)
    {
        target_ = attacker_;
    }
    bool far_from_destination = (std::abs(distance_to_target_.x) > range_ | std::abs(distance_to_target_.y) > range_);
    if (target_)
    {
        updateTargetCenter();
        distance_to_target_ = target_center_ - shape_center_;
        if (team_ != target_->get_team() && !far_from_destination && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_attack_time_).count() >= attack_speed_)
        {
            target_->set_attacker(shared_from_this());
            target_->reduceHealth(damage_);
            // std::cout << name_ << " attacking " << target_->get_name() << std::endl;

            last_attack_time_ = std::chrono::steady_clock::now();
        }
        if (target_->get_health() <= 0)
        {
            target_ = nullptr;
            if (attacker_)
            {
                attacker_ = nullptr;
            }
        }
    }
}

void Object::set_attacker(std::shared_ptr<Object> attacker){
    attacker_ = attacker;
};

void Object::handleFriendlyCollision(std::vector<std::shared_ptr<Object>> &objects, std::mt19937 &gen){
    if (!is_moving_ && can_move_){
        sf::FloatRect my_float_rect = get_float_rect();
        std::uniform_real_distribution<float> distribution(.02f, .1f);

        for (auto &&object : objects)
        {
            if (object != shared_from_this())
            {
                if (team_ == object->get_team() && my_float_rect.contains(object->get_shape_center()))
                {   
                    friendly_collision_adjusting_ = true;
                    sf::Vector2f distance_to_obj = object->get_shape_center() - shape_center_;
                    sf::Vector2f opposite_direction_to_obj = -distance_to_obj;
                    position_ = get_position();
                    sf::Vector2f random_pos = {distribution(gen), distribution(gen)};
                    sf::Vector2f next_pos = position_ + ((opposite_direction_to_obj) * .1f) + random_pos;
                    set_position(next_pos);
                    position_ = next_pos;
                    set_friendly_collision(true);
                    break;
                }
                else
                {
                    friendly_collision_adjusting_ = false;
                    // break; TODO test if i could break here
                }
            }
        }
    }
};

void Object::moveTowardsTarget() {
    is_moving_ = false;
    distance_to_target_ = target_center_ - shape_center_;
    bool far_from_destination = (std::abs(distance_to_target_.x) > 5.0f | std::abs(distance_to_target_.y) > 5.0f);
    if (can_move_ && (target_center_.x != 0.f && target_center_.y != 0.f)  && !friendly_collision_adjusting_) //TODO this one is to stop object when close to target - could do better
    {   
        if (target_) {
            updateTargetCenter();
            distance_to_target_ = target_center_ - shape_center_;
            if (std::abs(distance_to_target_.x) > 10.0f | std::abs(distance_to_target_.y) > 10.0f) {
                arrived_ = false;
            }
        }

        if (!arrived_) 
        {
            direction_to_target_ = distance_to_target_  / std::sqrt(distance_to_target_.x * distance_to_target_.x + distance_to_target_.y * distance_to_target_.y);
            
            if (far_from_destination) {
                position_ = get_position();
                is_moving_ = true;
            }
            //--path finding----
            if (!temp_traces_.empty())
            {
                sf::Vector2f temp_distance = temp_traces_[0] - shape_center_;
                sf::Vector2f temp_direction_to_target_ = temp_distance / std::sqrt(temp_distance.x * temp_distance.x + temp_distance.y * temp_distance.y);

                // log::d(temp_traces_[0].x, temp_traces_[0].y);
                sf::Vector2f next_pos = position_ + (temp_direction_to_target_ * speed_);
                set_position(next_pos);
                if (std::abs(temp_distance.x) < 1.0f && std::abs(temp_distance.y) < 1.0f)
                {
                    // log::ds("im close to temp_distance");
                    temp_traces_.erase(temp_traces_.begin());
                    // log::d(temp_traces_.size(), " traces count left");
                }
            }
            //---path finding----
            else
            {
                sf::Vector2f next_pos = position_ + (direction_to_target_ * speed_);
                set_position(next_pos);
            }

            if (!far_from_destination)
            {
                arrived_ = true;
                is_moving_ = false;
            }
        }
    }
};

bool Object::isLabourer()
{
    return labourer_;
}

void Object::findEnemies(std::vector<std::shared_ptr<Object>> *enemy_objects_ptr)
{
    if ((aggressive_ && !target_ && arrived_) || (team_ != 1 && aggressive_))
    {
        sf::Vector2f distance_to_target;
        bool far_from_target = true;
        for (auto &&object : *enemy_objects_ptr)
        {
            if (team_ != object->get_team())
            {
                distance_to_target = object->get_shape_center() - shape_center_;
                far_from_target = (std::abs(distance_to_target.x) > 60.0f | std::abs(distance_to_target.y) > 60.0f);

                if (!far_from_target)
                {
                    target_ = object;
                    break;
                }
            }
        }
    }
}

void Object::showPathToTarget()
{
    if (!arrived_)
    {
        // temp_path_.clear();
        temp_traces_.clear();
        temp_traces_ = PathFinder::findPath(shape_center_, target_center_);
        // for (auto &&vec : temp_traces_)
        // {
        //     sf::RectangleShape temp_rect;
        //     temp_rect.setSize({2.f, 2.f});
        //     temp_rect.setPosition(vec);
        //     temp_path_.push_back(temp_rect);
        // }
    }
    path_set_ = true;
};

void Object::updateObjectSpecifics(){

}; // this func can be orverriden by various object types according to their needs

void Object::update()
{

    while (threads_running_)
    {
        if (!path_set_ && can_move_)
        {
            showPathToTarget();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Object::checkVisibility(std::vector<std::shared_ptr<Object>> *enemy_objects_ptr)
{
    sf::Vector2f distance_to_target;
    bool far_from_target = true;
    for (auto &&object : *enemy_objects_ptr)
    {
        if (team_ != object->get_team())
        {
            distance_to_target = object->get_shape_center() - shape_center_;
            far_from_target = (std::abs(distance_to_target.x) > 100.0f | std::abs(distance_to_target.y) > 100.0f);

            if (!far_from_target)
            {
                visible_ = true;
                // log::ds("im visible");
                break;
            }
        }
    }
};