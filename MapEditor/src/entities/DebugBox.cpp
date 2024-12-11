//
// Created by allos on 12/11/2024.
//

#include <cstdio>
#include "DebugBox.h"

using namespace Tyche::Math;

void MapEntities::DebugBox::initialize() {
    _sprite = {1, 0};
    _scale = {16, 16};
    _position = {0, 0};
}

void MapEntities::DebugBox::update() {
    _position = Vector2{_AABB[0], _AABB[1]} + ((Vector2{_AABB[2], _AABB[3]} - Vector2{_AABB[0], _AABB[1]}) / 2);
    _scale =  (Vector2{_AABB[2], _AABB[3]} - Vector2{_AABB[0], _AABB[1]}) / 2;
}

void MapEntities::DebugBox::prepareRendering() {
    Entity::prepareRendering();
}

void MapEntities::DebugBox::destroy() {
    Entity::destroy();
}

void MapEntities::DebugBox::setBeginPos(const Vector2 &pos) {
    _AABB[0] = pos[0];
    _AABB[1] = pos[1];
}

void MapEntities::DebugBox::setEndPos(const Vector2 &pos) {
    _AABB[2] = pos[0];
    _AABB[3] = pos[1];
}
