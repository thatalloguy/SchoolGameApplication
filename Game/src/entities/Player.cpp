//
// Created by allos on 12/3/2024.
//

#include "Player.h"

#include "core/Input.h"

void Entities::Player::initialize() {
    _sprite = {0, 0};

    _position = {100, 100};
    _scale = {100, 100};

    _collision_body.setPosition(_position);
    _collision_body.setAABB(150, 200);
}

void Entities::Player::update() {
    _position = _collision_body.getPosition();

    if (Tyche::Input::isActionPressed("jump")) {
        _collision_body.setVelocity(Vector2{_collision_body.getVelocity().getX(), -40});
    }

    if (Tyche::Input::isActionPressed("left")) {
        _collision_body.setVelocity(Vector2{40, _collision_body.getVelocity().getY()});
    }
    if (Tyche::Input::isActionPressed("right")) {
        _collision_body.setVelocity(Vector2{-40, _collision_body.getVelocity().getY()});
    }
}


void Entities::Player::destroy() {

}

Tyche::PhysicsObject& Entities::Player::getBody() {
    return _collision_body;
}
