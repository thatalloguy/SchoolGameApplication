//
// Created by allos on 11/22/2024.
//

#include <cmath>
#include <cstdio>
#include "PhysicsHandler.h"

#include "spdlog/spdlog.h"

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

float Tyche::PhysicsHandler::distance(const Tyche::Math::Vector2 &a, const Tyche::Math::Vector2 &b) {
    return sqrt( sqrt(a.getX() - b.getX()) + sqrt(a.getY() - b.getY()));
}

Tyche::Math::Vector2 Tyche::PhysicsHandler::getCorrection(const Math::Vector2& aCenter, const Math::Vector2& bCenter, const AABB& A, const AABB& B) {

    Math::Vector2 correction{0, 0};


    if (Math::getDistance(aCenter.getY(), bCenter.getY()) >= Math::getDistance(aCenter.getX(), bCenter.getX())) {
        if (aCenter.getY() <= bCenter.getY()) {
            correction.setY(B[1] - A[3]);
        } else {
            correction.setY(B[3] - A[1]);
        }
    } else {
        if (aCenter.getX() <= bCenter.getX()) {
            correction.setX(B[0] - A[2]);
            spdlog::info("SMAL");
        } else {
            correction.setX(B[2] - A[0]);
            spdlog::info("BIG");
        }
    }

    return correction;
}

void Tyche::PhysicsHandler::resolveCollision(Tyche::PhysicsObject &a, Tyche::PhysicsObject &b) {

    Math::Vector2 normal;

    normal.setX(a.getVelocity().getX() - b.getVelocity().getX());
    normal.setY(a.getVelocity().getY() - b.getVelocity().getY());

    normal = normal.normalize();

    auto aVel = a.getVelocity();
    auto bVel = b.getVelocity();

    float totalMass = pow(a.getMass(), -1) + pow(b.getMass(), -1);

    auto contactVelocity = aVel - bVel;

    float restitution = fmin(a.getRestitution(), b.getRestitution());

    auto Vj = (  contactVelocity * -(1.0f + restitution)) * normal;

    auto j =  Vj / totalMass;



    a.setVelocity(-(normal * j * pow(a.getMass(), -1)));
    b.setVelocity(bVel - normal * j * pow(b.getMass(), -1));

    spdlog::info("V {} {}", a.getVelocity()[0], a.getVelocity()[1]);
    spdlog::info("N {} {} ", normal[0], normal[1]);

    //calc correction position

    auto aBox = a.getAABB();
    auto bBox = b.getAABB();

    Math::Vector2 aCenter = {aBox[0] + ((aBox[2] - aBox[0] ) / 2),aBox[1] + ((aBox[3] - aBox[1]) / 2)};
    Math::Vector2 bCenter = {bBox[0] + ((bBox[2] - bBox[0] ) / 2),bBox[1] + ((bBox[3] - bBox[1]) / 2)};

    Math::Vector2 correction = getCorrection(aCenter, bCenter, aBox, bBox);

    spdlog::info("Correction {} {}", correction[0], correction[1]);

    a.setCorrection(correction);
    b.setCorrection(-correction);
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

            if (PhysicsHandler::collision(a->getAABB(), b->getAABB())) {
                PhysicsHandler::resolveCollision(*a, *b);
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
