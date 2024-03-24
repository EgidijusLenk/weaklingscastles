
#include <SFML/Graphics.hpp>
#include "objectfactory.hpp"


ObjectFactory::ObjectFactory(sf::RenderWindow& window, std::chrono::steady_clock::time_point& time_now, Economy& eco) : window_(window), time_now_(time_now), eco_(eco)
{
    std::random_device rd;
    std::mt19937 gen_(rd());
}

ObjectFactory::~ObjectFactory()
{
}
std::shared_ptr<Object> ObjectFactory::createObject(const ObjectType &object_type, const int team, const sf::Vector2f &position)
{   
    std::vector<std::string> PEASANTWARRIORNAMES = { //TODO move to global #tech_debt++
    "Baker the Belligerent"
    "Blacksmith the Battle-Ready",
    "Cobbler the Combative",
    "Fletcher the Fearless",
    "Tavernkeeper the Tenacious",
    "Potter the Pugnacious",
    "Barber the Bold",
    "Scribe the Stalwart",
    "Brewer the Brave",
    "Weaver the Warrior",
    "Carpenter the Courageous",
    "Apothecary the Aggressive",
    "Tailor the Tough",
    "Miller the Militant",
    "Chandler the Challenger",
    "Tinker the Tenacious",
    "Farmer the Fearless",
    "Cooper the Courageous",
    "Miner the Mighty",
    "Falconer the Ferocious",
    "Jester the Juggernaut",
    "Wainwright the Warlord",
    "Tanner the Tumultuous",
    "Spinster the Spartan",
    "Dyer the Daring",
    "Ploughman the Powerful",
    "Mason the Marauder",
    "Groom the Gallant",
    "Weighman the Wily",
    "Sawyer the Savage"
};
    std::uniform_int_distribution<int> distribution(0, PEASANTWARRIORNAMES.size() - 1);
    std::string name = PEASANTWARRIORNAMES[distribution(gen_)];
    switch (object_type)
    {
    case ObjectType::Warrior:
        return std::make_shared<Warrior>(name, team, position, window_, time_now_, eco_);
        break;
        
    case ObjectType::Peasant:
        return std::make_shared<Peasant>(name, team, position, window_, time_now_, eco_);
        break;
        
    case ObjectType::Base:
        return std::make_shared<Base>(name, team, position, window_, time_now_, eco_);
        break;
    
    default:
        throw std::runtime_error("Error creating Object: no such object type exists");
        break;
    }
}
