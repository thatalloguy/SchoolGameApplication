//
// Created by allos on 11/20/2024.
//

#include "Camera.h"

Tyche::Camera::Camera(const Tyche::Math::Vector2& viewport_size): _viewport_size(viewport_size) { };

Tyche::Math::Matrix4 Tyche::Camera::getMatrix() const {
    return _camera_matrix;
}

void Tyche::Camera::setPosition(const Tyche::Math::Vector2 &new_position) {
    _position.setX(new_position.getX());
    _position.setY(new_position.getY());
}

Tyche::Math::Vector2 Tyche::Camera::getPosition() const {
    return _position;
}

Tyche::Math::Vector2 Tyche::Camera::getViewportSize() const {
    return _viewport_size;
}

void Tyche::Camera::setViewportSize(const Tyche::Math::Vector2 &new_size) {
    _viewport_size.setX(new_size.getX());
    _viewport_size.setY(new_size.getY());
}

void Tyche::Camera::update() {
    //First calculate the projection matrix and then translate (move) it to our position;
    _camera_matrix.ortho(0.0f, _viewport_size.getX(), _viewport_size.getY(), 0.0f, -1.0f, 1.0f);
    _camera_matrix.translate(_position);
}
