//
// Created by allos on 11/24/2024.
//

#include "PhysicsObject.h"

Tyche::PhysicsObject::PhysicsObject(const Tyche::Math::Vector2 &position, const Tyche::Math::Vector2 &init_vel,
                                    float mass): _position(position), _velocity(init_vel), _mass(mass) {};

void Tyche::PhysicsObject::step(float delta_time, Math::Vector2 gravity) {

    _force += gravity * _mass;

    _velocity += (_force / _mass) * delta_time;
    _position += _velocity * delta_time;

    _force = {0, 0};
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
    _aabb[0] = _position.getX();
    _aabb[1] = _position.getY();

    _aabb[2] = _aabb[0] + _size[0];
    _aabb[3] = _aabb[1] + (_size[1] * 1.5);

    return _aabb;
}

void Tyche::PhysicsObject::setPosition(const Tyche::Math::Vector2 &position) {
    _position = position;
}

void Tyche::PhysicsObject::setVelocity(const Tyche::Math::Vector2 &velocity) {
    _velocity = velocity;
}

void Tyche::PhysicsObject::setForce(const Tyche::Math::Vector2 &force) {
    _force = force;
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
