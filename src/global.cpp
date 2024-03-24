#include "global.hpp"


sf::Vector2f getObjectCenter(const sf::FloatRect &float_rect) {
    float center_x = float_rect.left + float_rect.width / 2.0f;
    float center_y = float_rect.top + float_rect.height / 2.0f;
    return {center_x, center_y};
};

bool isRadiusCollision(const sf::Vector2f& circle_center, float circle_cadius, const sf::Vector2f& point) {
    // Calculate the distance between the circle center and the point
    float distance = std::sqrt(std::pow(circle_center.x - point.x, 2) + std::pow(circle_center.y - point.y, 2));
    
    // Check if the distance is less than or equal to the circle radius
    return distance <= circle_cadius;
}

