//
// Created by allos on 11/22/2024.
//

#include <cmath>
#include <cstdio>
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

void Tyche::PhysicsHandler::ResolveCollision(Tyche::PhysicsObject &a, Tyche::PhysicsObject &b) {
    Math::Vector2 rv = b.getVelocity() - a.getVelocity();

    PhysicsCollision m {
        .a = &a,
        .b = &b
    };

    bool s = AABBvsAABB(m);

    auto aVel = a.getVelocity();
    auto bVel = b.getVelocity();

    a.setVelocity(aVel);
    b.setVelocity(bVel);

}

bool Tyche::PhysicsHandler::AABBvsAABB(Tyche::PhysicsCollision& physics_collision) {

    PhysicsObject* a = physics_collision.a;
    PhysicsObject* b = physics_collision.b;

    Math::Vector2 n = b->getPosition() - a->getPosition();

    AABB abox = a->getAABB();
    AABB bbox = b->getAABB();

    float a_extent = (abox[2] - abox[0]) / 2;
    float b_extent = (bbox[2] - bbox[0]) / 2;

    float x_overlap = a_extent + b_extent - abs(n.getX());

    if (x_overlap > 0) {
        a_extent = (abox[3] - abox[1]) / 2;
        b_extent = (bbox[3] - bbox[1]) / 2;

        float y_overlap = a_extent + b_extent - abs(n.getY());

        if (y_overlap > 0) {

            if (x_overlap > y_overlap) {
                if (n.getX() < 0) {
                    physics_collision.normal = {0, -1};
                } else {
                    physics_collision.normal = {0, 1};
                }
                return true;
            } else {
                if (n.getY() < 0) {
                    physics_collision.normal = {-1, 0};
                } else {
                    physics_collision.normal = {1, 0};
                }
                return true;
            }
        }
    }

    return false;
}
