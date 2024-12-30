//
// Created by allos on 12/21/2024.
//

#include "RoomEntity.h"

void Entities::RoomEntity::initialize(const Vector2& position, Game::Room *room, char tags[50]) {
    _parent = room;
    _position = position;

    _tag = tags;

}

bool Entities::RoomEntity::hasTag(const string &tag) {
    if (_tag == tag) {
        return true;
    }

    return false;
}


void Entities::RoomEntity::update(float delta_time) {
    Entity::update(delta_time);
}

void Entities::RoomEntity::destroy() {
    Entity::destroy();
}

void Entities::RoomEntity::reset() {
    //unused
}
