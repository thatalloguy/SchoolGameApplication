//
// Created by allos on 11/20/2024.
//

#include "Entity.h"


void Tyche::Entity::setPosition(const Tyche::Math::Vector2 &vec) {
    _position.setX(vec.getX());
    _position.setY(vec.getY());
}

void Tyche::Entity::setScale(const Tyche::Math::Vector2 &vec) {
    _scale.setX(vec.getX());
    _scale.setY(vec.getY());
}

Tyche::Math::Vector2 Tyche::Entity::getPosition() {
    return _position;
}

Tyche::Math::Vector2 Tyche::Entity::getScale() {
    return _scale;
}

void Tyche::Entity::calculateTransformMatrix() {
    _transform.translate(_position);
    _transform.scale(_scale);
}

Tyche::Math::Vector2 Tyche::Entity::getSprite() {
    return _sprite;
}

void Tyche::Entity::destroy() {

}

void Tyche::Entity::update(float delta_time) {
}

void Tyche::Entity::initialize() {

}

Tyche::Math::Matrix4 Tyche::Entity::getTransform() {
    return _transform;
}

void Tyche::Entity::prepareRendering() {
    calculateTransformMatrix();
}

void Tyche::Entity::setSprite(const Math::Vector2 &sprite) {
    _sprite = sprite;
}