//
// Created by allos on 12/3/2024.
//

#include "Player.h"

#include "core/Input.h"

void Entities::Player::initialize() {
    _sprite = {0, 0};

    _position = {300, 100};
    _scale = {50, 50};

    Vector4 collisionBody{100, -75, 0, 25};

    _collision_body = Tyche::PhysicsObject{collisionBody, Tyche::ObjectType::Rigid};
}

void Entities::Player::update() {
    _position = _collision_body.getPosition();
    _collision_body.setAABB({_position[0] - 50, _position[1] - 50, _position[0] + 50, _position[1] + 50});

    auto collision_info = _collision_body.getCollisionInfo();


    if (collision_info.is_colliding && collision_info.collision_normal[1] < 0 && Tyche::Input::isActionPressed("left")) {
        _collision_body.setVelocity(Vector2{40, _collision_body.getVelocity().getY()});
        //Unflip the sprite
        _scale.setX(50);
    }
    if (collision_info.is_colliding && collision_info.collision_normal[1] < 0 && Tyche::Input::isActionPressed("right")) {
        _collision_body.setVelocity(Vector2{-40, _collision_body.getVelocity().getY()});
        //Flip the sprite
        _scale.setX(-50);
    }

    //Check if we should jump
    if (collision_info.is_colliding &&
        collision_info.collision_normal[1] < -0.7 && Tyche::Input::isActionPressed("jump")) {
        _collision_body.setVelocity(Vector2{_collision_body.getVelocity().getX(), -40});

    }
}


void Entities::Player::destroy() {

}

Tyche::PhysicsObject& Entities::Player::getBody() {
    return _collision_body;
}
