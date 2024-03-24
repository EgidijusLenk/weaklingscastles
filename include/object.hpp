#pragma once
#include <memory>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "economy.hpp"
#include "pathfinder.hpp"
class Object : public std::enable_shared_from_this<Object>
{
private:
public:
    Object(std::string name, int team, sf::Vector2f position, sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco);
    ~Object();

    std::string name_; //TOOD add random name and display it!! very important
    int team_;
    float health_ = 100;
    float damage_;
    float speed_;
    float range_;
    unsigned int attack_speed_;
    bool aggressive_ = true;
    bool labourer_ = false;
    bool visible_ = false;

    Economy& eco_;
    sf::Text health_text_;
    sf::Font font_;
    sf::Vector2f position_ = { 0.f, 0.f}; //TODO This could be removed, bcs I can forget to update it, better use get_position(), but I use this to init, can solve, tho #tech_debt++
    std::shared_ptr<Object> target_;      // TODO I have double ownership: Only gameManager should be owning Objects. Use weak_ptr #tech_debt++
    std::shared_ptr<Object> attacker_;
    sf::Vector2f target_center_ = { 0.f, 0.f};

    sf::Vector2f distance_to_target_ = { 0.f, 0.f};
    sf::Vector2f direction_to_target_ = { 0.f, 0.f};
    sf::Vector2f shape_center_ = { 0.f, 0.f};
    sf::Vector2f base_position_;
    std::chrono::steady_clock::time_point &time_now_;
    std::chrono::steady_clock::time_point previous_time_; // TODO I can pass this from main.cpp, #tech_debt++ partly done in peasant resource collection (should copy for atacking) (NOT WORKING)
    std::chrono::steady_clock::time_point last_attack_time_;
    sf::RenderWindow& window_;

    bool selectable_ = true;
    bool selected_ = false;
    bool in_selection_ = false;
    
    bool can_move_ = true;
    bool is_moving_ = false;
    bool just_adjusted_friendly_collision_ = false;
    bool arrived_ = true;
    bool friendly_collision_adjusting_ = false;
    


    void setHealth(float _health);
    float get_health();
    std::string get_name();
    void drawHealth();
    void set_selected(bool selected);
    void setTarget(std::shared_ptr<Object> &target);
    void setTarget(sf::Vector2f &target);
    void removeTargetObj();
    void updateTargetCenter();
    void updateShapeCenter();
    int get_team();
    void set_friendly_collision(bool flag);
    bool get_friendly_collision();
    void reduceHealth(float amount);
    void handleDeath();
    void set_arrived(bool flag);
    void set_aggressive(bool flag);
    void handleAttack();
    void set_attacker(std::shared_ptr<Object> attacker);
    void handleFriendlyCollision(std::vector<std::shared_ptr<Object>> &objects, std::mt19937 &gen);
    void moveTowardsTarget();
    bool isLabourer();
    void findEnemies(std::vector<std::shared_ptr<Object>> *enemy_objects_ptr);

    //----------------path finder
    void showPathToTarget();
    std::vector<sf::Vector2f> temp_traces_;
    std::vector<sf::RectangleShape> temp_path_;
    bool threads_running_ = true;
    std::thread updateThread;
    sf::Vector2f previous_target_center_ = {0.f, 0.f};
    bool path_set_ = false;
    void update();
    void checkVisibility(std::vector<std::shared_ptr<Object>> *enemy_objects_ptr);
    //----------------path finder end

    virtual void drawObject() = 0;
    virtual sf::FloatRect get_float_rect() = 0;
    virtual sf::Vector2f get_shape_center() = 0;
    virtual sf::Vector2f get_position() = 0;
    virtual void set_position(sf::Vector2f position) = 0;
    virtual void set_shape_center() = 0;
    virtual void drawTargetline() = 0;
    virtual void updateObjectSpecifics(); // this func can be orverriden by various object types according to their needs
};

