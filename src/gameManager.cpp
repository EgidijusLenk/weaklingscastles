#include <algorithm>

#include "gameManager.hpp"

GameManager::GameManager(sf::RenderWindow &window, std::chrono::steady_clock::time_point time_now, int team) : eco_(), window_(window), time_now_(time_now), team_(team), of_(window, time_now, eco_), rf_(window, time_now), ui_(window, eco_), map_(window)
{
    selection_box_.setFillColor(sf::Color(100, 100, 255, 100));
    selection_box_.setOutlineColor(sf::Color::Blue);
    selection_box_.setOutlineThickness(2.f);
    std::cout << "GameManager created" << std::endl;
    std::random_device rd;
    std::mt19937 gen_(rd());
    previous_time_ = std::chrono::steady_clock::now();
    last_warrior_trained_ = previous_time_;
    last_peasant_trained_ = previous_time_;
    last_scenario_run_ = previous_time_;
    last_scenario_water_raid_ = previous_time_;
    last_scenario_gold_raid_ = previous_time_;
    last_scenario_base_raid_ = previous_time_;
    map_.convertPassableArea();
    // PathFinder::passable_area_ = map_.passable_area_; // TODO could do better #tech_debt++
}

GameManager::~GameManager()
{
    delete enemy_objects_ptr_;
    std::cout << "GameManager destoyed" << std::endl;
}

void GameManager::drawMap()
{
    map_.drawMap();
};
void GameManager::updateCurrentTime() {
    time_now_ = std::chrono::steady_clock::now();
};

void GameManager::startSelecting() {
    if (team_ == 1) // allow selection only for player1
    {
        if (!selected_objects_.empty())
        {
            for (auto &&object : selected_objects_)
            {
                object->set_selected(false);
            }
        }
        else if (!selected_resources_.empty())
        {
            for (auto &&resource : selected_resources_)
            {
                resource->set_selected(false);
            }
        }

        selected_objects_.clear();
        selected_resources_.clear();
        selecting_ = true;
        selection_start_ = getMousePosition();
        std::cout << "selection_start_ x " << selection_start_.x << " y " << selection_start_.y << std::endl;
        selection_box_.setPosition(selection_start_);
        selection_box_.setSize(sf::Vector2f(0, 0));
    }
};
bool GameManager::selecting() { //not using apparently
    return selecting_;
};

void GameManager::keepSelecting() {
    selection_end_ = getMousePosition();    
    selection_box_.setSize(selection_end_ - selection_start_);
};

void GameManager::endSelection() {
    selecting_ = false;
    selection_end_ = getMousePosition();
    selection_end_.x -= 0.1f; // quick fix to have volume if its just a click
    selection_box_.setSize(selection_end_ - selection_start_);

    // std::cout << "selection_end_ x " << selection_end_.x << " y " << selection_end_.y << std::endl;
    // std::cout << "selection_box_ pos x: " << selection_box_.getPosition().x << " y " << selection_box_.getPosition().y << std::endl;
    collectSelectedObjects();
        selection_box_.setSize({0.f, 0.f});

};

sf::Vector2f GameManager::getMousePosition() {
    float mouseX = static_cast<float>(sf::Mouse::getPosition(window_).x) /static_cast<float>(SCREEN_RESIZE);
	float mouseY = static_cast<float>(sf::Mouse::getPosition(window_).y) /static_cast<float>(SCREEN_RESIZE);
	// mousePosition = {mouseX, mouseY};
	return {mouseX, mouseY};

};

void GameManager::drawSelection() {
    window_.draw(selection_box_);
};

void GameManager::manageSelection(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {   
        bool clicked_on_button = false;
        bool clicked_on_ui = false;

        for (auto &&button : ui_.buttons_)
            if (button->getGlobalBounds().contains(getMousePosition())) {
                clicked_on_button = true;
                handleButtonAction(button->action_);
                break;
            }
        if (!clicked_on_button)
        {
            for (auto &&float_rect : ui_.static_dimensions_)
            {
                if (float_rect.contains(getMousePosition()))
                { 
                    clicked_on_ui = true;
                    break;
                }
                
            }
        }
        
        if (clicked_on_ui | clicked_on_button)
        {
            // std::cout << "mouse interacts with UI " << std::endl;
        } else
        {
            startSelecting();
        }
        
        
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && selecting_)
    {
        endSelection();

    }
    if (selecting_)
    {
        keepSelecting();
    }
    if (!selecting_ && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
    {
        if (selected_objects_.size() > 0)
        {        
        setTarget(event);
        }
        //handle right mouse button when clicked when no object is selected, fx to open menu
    }

};

void GameManager::createObject(const ObjectType &object_type, const int team, const sf::Vector2f &position) {
    std::shared_ptr<Object> temp_obj = of_.createObject(object_type, team, position);
    objects_.push_back(temp_obj);
};

void GameManager::createResource(const ResourceType &resource_type, const sf::Vector2f &position) {
    std::shared_ptr<Resource> temp_res = rf_.createResource(resource_type, position);
    resources_.push_back(temp_res);
};

void GameManager::drawObjects() {
    for (auto &&object : objects_){
        object->drawObject(); // comment this and uncomment the following in prod env
        // if (object->team_ == 1)
        // {
        //     object->drawObject();
        //     continue;
        // }
        // else if (object->visible_)
        // {
        //     object->drawObject();
        //     object->visible_ = false;
        // }
    }
};

void GameManager::drawResources() {
    for (auto &&resource : resources_){
        resource->drawResource();
    }
};

bool GameManager::checkSelectionCollision(const std::shared_ptr<Object>& object) {
    return selection_flaot_rect_.intersects(object->get_float_rect());
}
bool GameManager::checkSelectionCollision(const std::shared_ptr<Resource>& resource) {
    return selection_flaot_rect_.intersects(resource->get_float_rect());
}

void GameManager::collectSelectedObjects() {
    selection_flaot_rect_ = selection_box_.getGlobalBounds();

    auto collision_predicate = [this](const std::shared_ptr<Object> &object)
    {
        return checkSelectionCollision(object);
    };

    std::copy_if(objects_.begin(), objects_.end(), std::back_inserter(selected_objects_), collision_predicate);
    std::cout << "selected "<< selected_objects_.size() <<" objcets" << std::endl;

    for (auto &&object : selected_objects_)
    {
        object->set_selected(true);
    }
    if (selected_objects_.empty())
    {
        auto collision_predicate = [this](const std::shared_ptr<Resource> &resource)
        {
            return checkSelectionCollision(resource);
        };
        std::copy_if(resources_.begin(), resources_.end(), std::back_inserter(selected_resources_), collision_predicate);

        for (auto &&resource : selected_resources_)
        {
            
            resource->set_selected(true);
        }
    }
    
}


void GameManager::setTarget(sf::Event& event) {
        std::vector<std::shared_ptr<Object>> targetable_objects = {};
        sf::Vector2f mouse_position(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        for (auto &&object : *enemy_objects_ptr_)
        // for (auto &&object : objects_)
        {
            if (object->get_float_rect().contains(mouse_position))
            {
                targetable_objects.push_back(object);
            }
        }

        //assign random target from targetable_objects for each selected object
        std::uniform_int_distribution<int> distribution(0, targetable_objects.size() - 1);
        if (targetable_objects.size() > 0)
        {
            for (auto &&object : selected_objects_)
            {   
                object->setTarget(targetable_objects[distribution(gen_)]);

                // std::cout << "selected objcets moving to object targets" << std::endl;
            }
        }
        else 
        {   
            std::shared_ptr<Resource> target_resource = nullptr;
            for (auto &&resource : resources_)
            {
                if (resource->get_float_rect().contains(mouse_position))
                {   
                    target_resource = resource;
                    break;
                }
            }
            for (auto &&object : selected_objects_)
            {   
                object->removeTargetObj();
                object->set_arrived(false);
                
                if (target_resource && object->isLabourer())
                {
                    if (auto peasant = std::dynamic_pointer_cast<Peasant>(object))
                    {
                        peasant->collectResource(target_resource);
                        // std::cout << peasant->name_<<" peasant going to collect" << std::endl;
                    }
                }
                else
                {
                object->setTarget(mouse_position);
                // std::cout << "selected objects moving to mouse location" << std::endl;
                }
            }
            

        }
};

void GameManager::update() {
    time_now_ = std::chrono::steady_clock::now();
    for (auto &&object : objects_)
    {
        object->checkVisibility(enemy_objects_ptr_);
        object->updateObjectSpecifics();
        object->updateShapeCenter();
        object->moveTowardsTarget();
        object->findEnemies(enemy_objects_ptr_);
        object->handleFriendlyCollision(objects_, gen_);
    }
    for (auto it = objects_.begin(); it != objects_.end();)
    {
        auto object = *it;
        if (object->get_health() <= 0)
        {
            it = objects_.erase(it);
            std::cout << "one down!" << std::endl;
        }
        else
        {
            ++it;
        }
        object->handleAttack();
    }
    
    eco_.warriors_to_train_que_size_ = warriors_to_train_.size();
    eco_.peasants_to_train_que_size_ = peasants_to_train_.size();

    trainWarrior();
    trainPeasant();
    eco_.set_population(static_cast<float>(objects_.size()));
    consumeWater();
};

void GameManager::drawUI(){
    ui_.updateUI();
    ui_.drawMainUI();
}

void GameManager::handleButtonAction(ButtonAction action){
    sf::Vector2f rally_point = RALLY_POINT;
    if (team_ != 1)
    {
        rally_point = RALLY_POINT_2;
    }
    switch (action)
    {
    case ButtonAction::CreateWarrior:

        warriors_to_train_.push(queObject(ObjectType::Warrior, team_, rally_point));
        last_warrior_trained_ = std::chrono::steady_clock::now();
        break;
    case ButtonAction::CreatePeasant:
        peasants_to_train_.push(queObject(ObjectType::Peasant, team_, rally_point));
        last_peasant_trained_ = std::chrono::steady_clock::now();
        break;
    case ButtonAction::CreateArcher:

        break;

    case ButtonAction::Null:
        break;
    
    default:
        break;
    }
};

std::shared_ptr<Object> GameManager::queObject(const ObjectType &object_type, const int team, const sf::Vector2f &position){
    return of_.createObject(object_type, team, position);
};

void GameManager::trainWarrior(){
    if (!warriors_to_train_.empty())
    {

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_warrior_trained_).count() >= WARRIOR_PRODUTION_TIME && eco_.decreaseGold(WARRIOR_COST))
        {
            log::ds("warrior need run");
            objects_.push_back(warriors_to_train_.front());
            last_warrior_trained_ = std::chrono::steady_clock::now();
            // std::cout << warriors_to_train_.front()->get_name() <<" warrior trained" << std::endl;
            warriors_to_train_.pop();
        }
    }
};

void GameManager::trainPeasant(){
    if (!peasants_to_train_.empty())
    {

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_peasant_trained_).count() >= PEASANT_PRODUTION_TIME && eco_.decreaseGold(PEASANT_COST))
        {
            log::ds("peasant need run");
            objects_.push_back(peasants_to_train_.front());
            last_peasant_trained_ = std::chrono::steady_clock::now();
            std::cout << peasants_to_train_.front()->get_name() << " peasant trained" << std::endl;
            peasants_to_train_.pop();
        }
    }
};

void GameManager::consumeWater(){
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_comsumed_water_).count() >= water_consumption_period) 
    {
        if (!eco_.decreaseWater(objects_.size() * WATER_CONSUMPTION))
        {
            eco_.set_water(0.f);
            // std::cout << "WATER RUN OUT! PEOPLE DYING :)" << std::endl;

            water_consumption_period = KILL_OBJECT_PERIOD;
            if (!objects_.empty())
            {   
                objects_.pop_back();
            }
        }
        else if (eco_.get_water() > 0.f)
            {
                water_consumption_period = WATER_CONSUMPTION_PERIOD;
            }
        last_comsumed_water_ = std::chrono::steady_clock::now();
    }
};

//-------------------botScenario-------------------
void GameManager::botScenario()
{
    if (team_ != 1 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_scenario_run_).count() >= 5000) // ms
    {
        std::vector<std::shared_ptr<Object>> labourers;
        std::vector<std::shared_ptr<Object>> warriors;

        std::copy_if(objects_.begin(), objects_.end(), std::back_inserter(labourers), [](std::shared_ptr<Object> obj)
                     { return obj->labourer_; });
        std::copy_if(objects_.begin(), objects_.end(), std::back_inserter(warriors), [](std::shared_ptr<Object> obj)
                     { return !(obj->labourer_); });

        unsigned int warrior_count = warriors.size();
        unsigned int peasant_count = labourers.size();
        //  log::d("gold:", eco_.get_gold(), "water:", eco_.get_water(), "warriors:", warrior_count, "peasants:", peasant_count);
        //  eiti mininti +
        //  gaminti karius ir peasants 1/8 santykiu +
        //  eiti saugoti resursu+
        //  pulti prieso baze+
        //  pulti prieso resursus+
        //  gintis jei mano baze musa +-
        //  padaryti kad neitu selectinti prieso +

        sf::Vector2f target1 = {MAP_WIDTH / 2 + 30.f,
                                MAP_HEIGHT / 2};
        sf::Vector2f target2 = {1160.f, 436.f};
        sf::Vector2f target3 = {MAP_WIDTH / 2 - 30.f,
                                580.f};
        sf::Vector2f target4 = {940.f, 250.f};
        sf::Vector2f target5 = {520.f, 500.f};
        sf::Vector2f target6 = {520.f, 500.f};
        sf::Vector2f target7 = {BASE_POSITION_TEAM_2.x - 70.f, BASE_POSITION_TEAM_2.y - 80.f};

        std::vector<sf::Vector2f> patrol_targets{target1, target2, target3, target4, target5, target6, target7};
        std::shuffle(patrol_targets.begin(), patrol_targets.end(), gen_);

        for (size_t i = 0; i < warrior_count; i++)
        {
            if (!warriors[i]->target_ && warriors[i]->arrived_)
            {

                warriors[i]->set_arrived(false);
                warriors[i]->setTarget(patrol_targets[i % patrol_targets.size()]);
            }
        }

        // attack enemy water resource
        std::uniform_int_distribution<int> distribution(15000, 40000);
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_scenario_water_raid_).count() >= distribution(gen_)) // ms
        {
            sf::Vector2f enemy_water_res = {38.f, 345.f};

            std::uniform_int_distribution<int> distribution(0, warriors.size() - 1);
            for (size_t i = 0; i <= distribution(gen_); i++)
            {
                warriors[i]->set_arrived(false);
                warriors[i]->setTarget(enemy_water_res);
                // free warriors
                if (enemy_objects_ptr_->size() >= warrior_count * 1.8)
                {
                    createObject(ObjectType::Warrior, 2, target7);
                }
            }

            last_scenario_water_raid_ = std::chrono::steady_clock::now();
        }

        // attack enemy gold resource
        std::uniform_int_distribution<int> distribution_2(4000, 12000);
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_scenario_gold_raid_).count() >= distribution_2(gen_)) // ms
        {
            // get free gold
            eco_.increaseGold(50.f);
            sf::Vector2f enemy_gold_res = {MAP_WIDTH / 2 - 45.f, MAP_HEIGHT / 2 - 25.f};

            std::uniform_int_distribution<int> distribution(0, warriors.size() - 1);
            for (size_t i = 0; i <= distribution(gen_); i++)
            {
                warriors[i]->set_arrived(false);
                warriors[i]->setTarget(enemy_gold_res);
            }
            last_scenario_gold_raid_ = std::chrono::steady_clock::now();
        }

        // attack enemy base
        std::uniform_int_distribution<int> distribution_3(300000, 400000);
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_scenario_base_raid_).count() >= distribution_3(gen_)) // ms
        {
            sf::Vector2f enemy_base = {BASE_POSITION.x + 115.f, BASE_POSITION.y + 118.f};
            for (auto &&warrior : warriors)
            {
                warrior->set_arrived(false);
                warrior->setTarget(enemy_base);
            }
            last_scenario_base_raid_ = std::chrono::steady_clock::now();
        }

        // produce peasants
        // if (warrior_count / peasant_count > 2) // lets say peasant to warrior ratio is 1 to 8
        // {
        //     log::ds("need peasants");
        //     handleButtonAction(ButtonAction::CreatePeasant);
        // }

        // produce peasant and direct one to gather water
        if (eco_.get_water() < 50.f)
        {
            log::ds("need water");
            handleButtonAction(ButtonAction::CreatePeasant);

            // labourers.back()->removeTargetObj();
            // labourers.back()->set_arrived(false);
            // std::dynamic_pointer_cast<Peasant>(labourers.back())->collectResource(resources_[1]);
            // peasant_collecting_water = true;
        }

        // produce warriors
        if (warriors_to_train_.size() <= 2)
        {
            handleButtonAction(ButtonAction::CreateWarrior);
            handleButtonAction(ButtonAction::CreatePeasant);
        }

        // assign resource for pesants
        for (size_t i = 0; i < peasant_count; i++)
        {
            if (!std::dynamic_pointer_cast<Peasant>(labourers[i])->is_in_collection_cycle())
            {
                labourers[i]->removeTargetObj();
                labourers[i]->set_arrived(false);
                if (eco_.get_water() < 200.f)
                {
                    std::dynamic_pointer_cast<Peasant>(labourers[i])->collectResource(resources_[1]); // resource water
                }
                else
                {
                    std::dynamic_pointer_cast<Peasant>(labourers[i])->collectResource(resources_[0]); // resource gold
                }
            }
        }

        last_scenario_run_ = std::chrono::steady_clock::now();
        std::cout << eco_.get_water() << " water" << std::endl;
        // std::cout << eco_.average_water_production_ << " prod" << std::endl;
        // std::cout << eco_.get_gold() << " gold" << std::endl;
        std::cout << peasants_to_train_.size() << " peasants_to_train_" << std::endl;
        std::cout << warriors_to_train_.size() << " warriors_to_train_" << std::endl;
    }
}

;