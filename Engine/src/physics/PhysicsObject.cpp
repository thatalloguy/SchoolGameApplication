//
// Created by allos on 11/24/2024.
//

#include "PhysicsObject.h"

Tyche::PhysicsObject::PhysicsObject(const Tyche::Math::Vector2 &position, const Tyche::Math::Vector2 &init_vel,
                                    float mass): _position(position), _velocity(init_vel), _mass(mass) {}

Tyche::PhysicsObject::PhysicsObject(Tyche::Math::Vector4& AABB, ObjectType type) {

    _position = {AABB[0] + ((AABB[2] - AABB[0] ) / 2),AABB[1] + ((AABB[3] - AABB[1]) / 2)};

    _size.setX(AABB[2] - AABB[0]);
    _size.setY(AABB[3] - AABB[1]);

    _type = type;
}

void Tyche::PhysicsObject::step(float delta_time, Math::Vector2 gravity) {

    // Calc the force from the gravity; F = m * G;
    _force += gravity * _mass;

    // calc our new velocity, V+ = F/m
    _velocity += (_force / _mass) * delta_time;

    //Reset our force;
    _force = {0, 0};

    // Add a correction (if we have one)
    _position += _correction;

    // Update our position according to our velocity
    _position += _velocity * delta_time;

    // Reset the correction;
    _correction = {0, 0};
}


Tyche::Math::Vector2 Tyche::PhysicsObject::getPosition() {
    return _position;
}

Tyche::Math::Vector2 Tyche::PhysicsObject::getVelocity() {
    return _velocity;
}

Tyche::Math::Vector2 Tyche::PhysicsObject::getForce() {
    return _force;
}

float Tyche::PhysicsObject::getMass() {
    return _mass;
}

Tyche::Math::Vector4 Tyche::PhysicsObject::getAABB() {
    //Update our AABB
    // ([0],[1]) ------- ([2],[1])
    // |                    |
    // |                    |
    // |                    |
    // |                    |
    // ([0],[3]) ------  ([2],[3])

    _aabb[0] = _position.getX();
    _aabb[1] = _position.getY();

    _aabb[2] = _position.getX() + _size[0];
    _aabb[3] = _position.getY() + _size[1];

    return _aabb;
}

void Tyche::PhysicsObject::setPosition(const Tyche::Math::Vector2 &position) {
    _position = position;
}

void Tyche::PhysicsObject::setVelocity(const Tyche::Math::Vector2 &velocity) {
    _velocity = velocity;
}

void Tyche::PhysicsObject::setForce(const Tyche::Math::Vector2 &force) {
    _velocity = force;
}

void Tyche::PhysicsObject::setMass(float mass) {
    _mass = mass;
}

void Tyche::PhysicsObject::setAABB(float w, float h) {
    _aabb[0] = _position.getX();
    _aabb[1] = _position.getY();

    _size.setX(w);
    _size.setY(h);

    _aabb[2] = _aabb[0] + w;
    _aabb[3] = _aabb[1] + h;
}

void Tyche::PhysicsObject::setRestitution(float restitution) {
    _restitution = restitution;
}

float Tyche::PhysicsObject::getRestitution() {
    return _restitution;
}

Tyche::PhysicsID Tyche::PhysicsObject::getID() {
    return _id;
}

void Tyche::PhysicsObject::setID(Tyche::PhysicsID id) {
    _id = id;
}

Tyche::ObjectType Tyche::PhysicsObject::getObjectType() {
    return _type;
}

void Tyche::PhysicsObject::setObjectType(Tyche::ObjectType type) {
    _type = type;
}

void Tyche::PhysicsObject::addForce(const Tyche::Math::Vector2 &vel) {
    _force -= vel;
}

void Tyche::PhysicsObject::setCorrection(const Tyche::Math::Vector2 &cor) {
    _correction = cor;
}

Tyche::CollisionInfo Tyche::PhysicsObject::getCollisionInfo() {
    CollisionInfo info{_col_info.is_colliding, _col_info.collision_normal};
    _col_info = {false, {0, 0}};

    return info;
}

void Tyche::PhysicsObject::setCollisionInfo(const CollisionInfo& new_info) {
    _col_info = new_info;
}
