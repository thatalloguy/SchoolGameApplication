//
// Created by allos on 11/22/2024.
//

#include <cmath>
#include "PhysicsHandler.h"


bool Tyche::PhysicsHandler::collision(const Tyche::AABB &a, const Tyche::AABB &b) {

    // [0] = min.x, [1] = min.y
    // [2] = max.x, [3] = max.y

    // Collision detection using the seperate axis theory.
    // We check if any axis are seperated which automatically means no collision, so we return false
    if (a[2] < b[0] || a[0] > b[2]) return false;
    if (a[3] < b[1] || a[1] > b[3]) return false;

    // If both axes arent seperated it means we have a collision
    return true;
}


Tyche::Math::Vector2 Tyche::PhysicsHandler::getCorrection(const Math::Vector2& aCenter, const Math::Vector2& bCenter, const AABB& A, const AABB& B) {

    Math::Vector2 correction{0, 0};

    // First check what axis we should correct (the one most far away from each other)
    if (bCenter.getY() <= A[1] && bCenter.getY() <= A[3]) {
        // Check if A is above or below B
        if (aCenter.getY() <= bCenter.getY()) {
            correction.setY(B[1] - A[3]);
        } else {
            correction.setY(B[3] - A[1]);
        }
    } else {
        // Check if A is left or right from B
        if (aCenter.getX() <= bCenter.getX()) {
            correction.setX(B[0] - A[2]);
        } else {
            correction.setX(B[2] - A[0]);
        }
    }

    return correction;
}

void Tyche::PhysicsHandler::resolveCollision(Tyche::PhysicsObject &a, Tyche::PhysicsObject &b) {

    if (a.getObjectType() == ObjectType::Static &&
        b.getObjectType() == ObjectType::Static ||
        a.getObjectType() == ObjectType::Ghost ||
        b.getObjectType() == ObjectType::Ghost)
        return;

    Math::Vector2 normal;

    // n = normalize(bVel - aVel)
    normal.setX(b.getVelocity().getX() - a.getVelocity().getX());
    normal.setY(a.getVelocity().getY() - b.getVelocity().getY());

    normal = normal.normalize();

    if (normal.getX() > 1 || normal.getX() < -1 ||
        normal.getY() > 1 || normal.getY() < -1 ) {
        spdlog::warn("Invalid collision normal");
        return;
    }

    auto aVel = a.getVelocity();
    auto bVel = b.getVelocity();

    // The relative velocity
    auto rv = bVel - aVel;

    // The velocity we should have along the collision
    float velAlongNormal = Tyche::Math::dot(rv, normal);

    // get the smallest resititution
    float e = fmin(a.getRestitution(), b.getRestitution());

    // j = -(1 + e) * vel / totalmass;
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / a.getMass() + 1 / b.getMass();

    Math::Vector2 impulse =  normal * j;

    // DO NOT REMOVE ELSE EVERYTHING BREAKS
    b.setVelocity(b.getVelocity() + (impulse * (1 / b.getMass())));


    //apply velocities only to rigidbodies
    if (a.getObjectType() == ObjectType::Rigid) {
        a.setVelocity(a.getVelocity() - (impulse * (1 / a.getMass())));
    }

    if (b.getObjectType() == ObjectType::Rigid) {
        b.setVelocity(b.getVelocity() + (impulse * (1 / b.getMass())));
    }

    // Calculate and apply corrections
    auto aBox = a.getAABB();
    auto bBox = b.getAABB();

    Math::Vector2 aCenter = {aBox[0] + ((aBox[2] - aBox[0] ) / 2),aBox[1] + ((aBox[3] - aBox[1]) / 2)};
    Math::Vector2 bCenter = {bBox[0] + ((bBox[2] - bBox[0] ) / 2),bBox[1] + ((bBox[3] - bBox[1]) / 2)};


    Math::Vector2 correction = getCorrection(aCenter, bCenter, aBox, bBox);

    if (a.getObjectType() == ObjectType::Rigid)
        a.setCorrection(-correction);
    if (b.getObjectType() == ObjectType::Rigid)
        b.setCorrection(-correction);

    a.setCollisionInfo({true, normal});
    b.setCollisionInfo({true, normal});

}

Tyche::World::World(bool handle_memory, Tyche::Math::Vector2 gravity) : _gravity(gravity), _should_handle_memory(handle_memory) {

}

Tyche::World::~World() {
    if (_should_handle_memory) {
        for (auto body: _bodies) {
            delete body;
        }
    }
}

Tyche::PhysicsID Tyche::World::addRigidBody(Tyche::PhysicsObject *object) {
    // Increase the id count
    _id_count++;

    // Update the object's information
    object->setID(_id_count);
    object->setObjectType(ObjectType::Rigid);

    _bodies.push_back(object);

    return _id_count;
}

Tyche::PhysicsID Tyche::World::addStaticBody(Tyche::PhysicsObject *object) {
    // Increase the id count
    _id_count++;

    // Update the object's information
    object->setID(_id_count);
    object->setObjectType(ObjectType::Static);

    _bodies.push_back(object);

    return _id_count;
}


Tyche::PhysicsID Tyche::World::addGhostBody(Tyche::PhysicsObject *object) {
    // Increase the id count
    _id_count++;

    // Update the object's information
    object->setID(_id_count);
    object->setObjectType(ObjectType::Ghost);

    _bodies.push_back(object);

    return _id_count;
}


void Tyche::World::removeBody(Tyche::PhysicsID id) {
    for (int i=0; i<_bodies.length(); i++) {
        if (_bodies[i]->getID() == id) {
            if (_should_handle_memory) {
                delete _bodies[i];
            }
            _bodies.remove(i);
        }
    }
}



void Tyche::World::step(float delta_time) {
    //First  Update all of our rigidBodies;
    for (auto body: _bodies) {
        if (body->getObjectType() == ObjectType::Rigid) {
            body->step(delta_time, _gravity);
        }
    }

    // Then check for collision and resolve them if needed
    for (int i=0; i<_bodies.length(); i++) {
        auto a = _bodies[i];
        if (a->getObjectType() == ObjectType::Ghost)
            break;

        // Loop through all of the other objects, since we have to check against every other body
        // Isnt optimale but we wont have so much bodies in our vector so it wont be a issue
        for (auto b: _bodies) {
            if (b->getID() == a->getID())
                break;

            if (PhysicsHandler::collision(a->getAABB(), b->getAABB())) {
                PhysicsHandler::resolveCollision(*a, *b);
            }
        }
    }
}

void Tyche::World::renderBodies(Tyche::DebugRenderer& debug_renderer) {
    for (auto body: _bodies) {
        debug_renderer.renderBox(body->getAABB());

        if (body->getVelocity() != Vector2{0, 0}) {
            auto vel_normal = body->getVelocity();
            vel_normal.normalize();

            vel_normal *= 3.0f;
            vel_normal += body->getPosition();

            debug_renderer.renderLine(body->getPosition(), vel_normal);
        }

    }
}

Tyche::Math::Vector2 Tyche::World::getGravity() {
    return _gravity;
}

void Tyche::World::setGravity(const Tyche::Math::Vector2 &vec) {
    _gravity = vec;
}
