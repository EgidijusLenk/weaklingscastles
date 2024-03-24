#include <SFML/Graphics.hpp>
#include <chrono>
#include <ctime>
#include <vector>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <math.h>

#include "global.hpp"
#include "gameManager.hpp"
#include "pathfinder.hpp"
#include "objectfactory.hpp"
#include "object.hpp"
#include "base.hpp"
#include "warrior.hpp"

int main()
{
    sf::Event event;
    bool is_paused = true;
    bool game_won = false;
    bool game_lost = false;
    unsigned lag = 0;
    std::chrono::steady_clock::time_point previous_time = std::chrono::steady_clock::now();
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT), "Weaklings Castles 0.0.1");
    GameManager gm(window, previous_time, 1);
    GameManager gm2(window, previous_time, 2);
    PathFinder::passable_area_ = gm.map_.passable_area_; // TODO could do better #tech_debt++

    //------------------------------------------------
    gm.createObject(ObjectType::Base, 1, BASE_POSITION);
    gm2.createObject(ObjectType::Base, 2, BASE_POSITION_TEAM_2);

    gm.createObject(ObjectType::Warrior, 1, {150.f, 100.f});
    gm.createObject(ObjectType::Warrior, 1, {145.f, 120.f});
    // gm.createObject(ObjectType::Warrior, 1, {110.f, 100.f});
    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});
    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});

    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});
    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});
    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});
    // gm.createObject(ObjectType::Warrior, 1, {190.f, 170.f});
    // gm.createObject(ObjectType::Warrior, 1, {120.f, 170.f});
    gm2.createObject(ObjectType::Warrior, 2, {800.f, 450.f});
    gm2.createObject(ObjectType::Warrior, 2, {800.f, 450.f});
    gm2.createObject(ObjectType::Warrior, 2, {800.f, 450.f});
    gm2.createObject(ObjectType::Warrior, 2, {800.f, 450.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm2.createObject(ObjectType::Warrior, 2, {600.f, 700.f});
    // gm.createResource(ResourceType::Gold, {610.f, 200.f});
    gm.createResource(ResourceType::Gold, {MAP_WIDTH / 2 - 25.f, MAP_HEIGHT / 2 - 25.f});
    gm.createResource(ResourceType::Water, {MAP_WIDTH - 70.f, 325.f});
    gm.createResource(ResourceType::Water, {20.f, 345.f});

    gm.createObject(ObjectType::Peasant, 1, {120.f, 100.f});
    gm.createObject(ObjectType::Peasant, 1, {120.f, 100.f});
    gm2.createObject(ObjectType::Peasant, 2, {800.f, 450.f});
    gm2.createObject(ObjectType::Peasant, 2, {800.f, 450.f});

    // spinning rectangle:

    sf::RectangleShape rectangle(sf::Vector2f(5, 20));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOrigin(rectangle.getSize() / 2.0f);
    rectangle.setPosition(window.getSize().x * 0.98f, window.getSize().y * 0.98f);
    float rotationSpeed = 4.0f;

    sf::Font font;
    font.loadFromFile("resources/Arial.ttf");

    // game time:
    sf::Text gameLengthText;
    gameLengthText.setFont(font);
    gameLengthText.setCharacterSize(16);
    gameLengthText.setFillColor(sf::Color::White);
    gameLengthText.setPosition(10.f, MAP_HEIGHT - 25.f);
    sf::Clock clock;
    // endscreen:
    sf::Text end_text;
    end_text.setFont(font);
    end_text.setString("Victory! What a milestone for humanity...");
    end_text.setCharacterSize(40);
    end_text.setFillColor(sf::Color::Green);
    end_text.setPosition(200, 250);
    //------------------------------------------------

    gm2.resources_ = gm.resources_; // Just copying because happens only on program init. Not great not terrible.
    gm2.resources_.pop_back();
    gm.enemy_objects_ptr_ = &gm2.objects_;
    gm2.enemy_objects_ptr_ = &gm.objects_;

    while (window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;
            while (1 == window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                gm.manageSelection(event);
                gm2.manageSelection(event);
            }

            // if (window.hasFocus())
            if (1)
            {
                is_paused = false;
                game_won = gm2.objects_.empty();
                game_lost = gm.objects_.empty();

                // use for draw
                if (FRAME_DURATION > lag)
                {
                    // Calculate game length
                    sf::Time elapsedTime = clock.getElapsedTime();
                    int totalSeconds = static_cast<int>(elapsedTime.asSeconds());

                    int hours = totalSeconds / 3600;
                    int minutes = (totalSeconds % 3600) / 60;
                    int seconds = totalSeconds % 60;

                    // Update game length text
                    std::stringstream ss;
                    ss << std::setfill('0') << std::setw(2) << hours << ":";
                    ss << std::setfill('0') << std::setw(2) << minutes << ":";
                    ss << std::setfill('0') << std::setw(2) << seconds;

                    gameLengthText.setString(ss.str());

                    gm2.botScenario();
                    rectangle.rotate(rotationSpeed);
                    window.clear();
                    window.draw(rectangle); // green spinning thing
                    gm.drawMap();
                    gm.update();
                    gm2.update();
                    gm.drawResources();
                    gm2.drawObjects();
                    gm.drawObjects();
                    gm.drawSelection();
                    // gm2.drawSelection();
                    gm.drawUI();
                    if (game_won)
                    {
                        window.clear(sf::Color::Black);
                        window.draw(end_text);
                    }
                    else if (game_lost)
                    {
                        end_text.setString("It`s ok. Games are hard.");
                        window.clear(sf::Color::Black);
                        window.draw(end_text);
                    }
                    window.draw(gameLengthText);
                    window.display();
                    // log::d(gm2.eco_.get_water());
                }
            }
            else
            {
                is_paused = true;
            }

        } // main loop end
    }

    return 0;
}