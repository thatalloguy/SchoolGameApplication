//
// Created by allos on 11/22/2024.
//

#include <cmath>
#include <cstdio>
#include "PhysicsHandler.h"

#include "spdlog/spdlog.h"

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

    spdlog::info(" B {}", a.getVelocity().getY());

    PhysicsCollision m {
        .a = &a,
        .b = &b
    };

    Math::Vector2 normal;

    normal.setX(a.getVelocity().getX() - b.getVelocity().getX());
    normal.setY(a.getVelocity().getY() - b.getVelocity().getY());

    normal = normal.normalize();

    normal *= -1;

    auto aVel = a.getVelocity();
    auto bVel = b.getVelocity();

    float totalMass = pow(a.getMass(), -1) + pow(b.getMass(), -1);

    auto contactVelocity = aVel - bVel;

    float restitution = fmin(a.getRestitution(), b.getRestitution());

    auto j = (  contactVelocity * -(1.0f + restitution)) / totalMass;



    a.setVelocity(-(normal * j * pow(a.getMass(), -1)));
    b.setVelocity(bVel - normal * j * pow(b.getMass(), -1));


    spdlog::info(" N {} {}", normal[0], normal[1]);

}

bool Tyche::PhysicsHandler::AABBvsAABB(Tyche::PhysicsCollision& physics_collision) {

    PhysicsObject* a = physics_collision.a;
    PhysicsObject* b = physics_collision.b;

    Math::Vector2 n = b->getPosition() - a->getPosition();


    AABB abox = a->getAABB();
    AABB bbox = b->getAABB();

    physics_collision.correction = {bbox[2] - abox[0], bbox[3] - abox[1]};

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

Tyche::World::World(Tyche::Math::Vector2 gravity) : _gravity(gravity) {

}

Tyche::World::~World() {

}

Tyche::PhysicsID Tyche::World::addRigidBody(Tyche::PhysicsObject *object) {
    _id_count++;

    object->setID(_id_count);
    object->setObjectType(ObjectType::Rigid);

    _bodies.push_back(object);

    return _id_count;
}

Tyche::PhysicsID Tyche::World::addStaticBody(Tyche::PhysicsObject *object) {
    _id_count++;

    object->setID(_id_count);
    object->setObjectType(ObjectType::Static);

    _bodies.push_back(object);

    return _id_count;
}


void Tyche::World::removeBody(Tyche::PhysicsID id) {
    for (int i=0; i<_bodies.length(); i++) {
        if (_bodies[i]->getID() == id) {
            _bodies.remove(i);
        }
    }
}



void Tyche::World::step(float delta_time) {
    // Update all of our rigidBodies;
    for (auto body: _bodies) {
        if (body->getObjectType() == ObjectType::Rigid) {
            body->step(delta_time, _gravity);
        }
    }


    for (int i=0; i<_bodies.length(); i++) {
        auto a = _bodies[i];

        for (auto b: _bodies) {
            if (b->getID() == a->getID())
                break;

            if (PhysicsHandler::isColliding(a->getAABB(), b->getAABB())) {
                PhysicsHandler::ResolveCollision(*a, *b);
            }
        }
    }

    for (auto body: _bodies) {
        if (body->getObjectType() == ObjectType::Rigid)
            body->update(delta_time);
    }
}

Tyche::Math::Vector2 Tyche::World::getGravity() {
    return _gravity;
}

void Tyche::World::setGravity(const Tyche::Math::Vector2 &vec) {
    _gravity = vec;
}
