//
// Created by allos on 11/22/2024.
//

#include <cmath>
#include "PhysicsHandler.h"

bool Tyche::PhysicsHandler::isColliding(const Tyche::AABB &a, const Tyche::AABB &b) {
    // [0] = min.x, [1] = min.y
    // [2] = max.x, [3] = max.y

    // Collision detection using the seperate axis theory.
    // We check if any axis are seperated which automatically means no collision, so we return false
    if (a[2] < b[0] || a[0] > b[2]) return false;
    if (a[3] < b[1] || a[1] > b[3]) return false;

    // If both axes arent seperated it means we have a collision
    return true;
}

float Tyche::PhysicsHandler::distance(const Tyche::Math::Vector2 &a, const Tyche::Math::Vector2 &b) {
    return sqrt( sqrt(a.getX() - b.getX()) + sqrt(a.getY() - b.getY()));
}
