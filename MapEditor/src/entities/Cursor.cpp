//
// Created by allos on 12/5/2024.
//

#include "Cursor.h"

void MapEntities::Cursor::initialize() {
    _sprite = {3, 0};
    _scale = {30, 30};

    Entity::initialize();
}

void MapEntities::Cursor::update() {
    Entity::update();
}

void MapEntities::Cursor::prepareRendering() {
    Entity::prepareRendering();
}

void MapEntities::Cursor::destroy() {
    Entity::destroy();
}

void MapEntities::Cursor::updateSprite(const Tyche::Math::Vector2& sprite) {
    _sprite = sprite;
}
