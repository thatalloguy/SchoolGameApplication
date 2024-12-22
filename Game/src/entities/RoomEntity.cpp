//
// Created by allos on 12/21/2024.
//

#include "RoomEntity.h"

void Entities::RoomEntity::initialize(const Vector2& position, Game::Room *room, char tags[50]) {
    _parent = room;
    _position = position;

    loadTagsFromSingleString(tags);

}

bool Entities::RoomEntity::hasTag(const string &tag) {
    for (auto& _tag: _tags) {
        if (_tag == tag) {
            return true;
        }
    }

    return false;
}

void Entities::RoomEntity::loadTagsFromSingleString(char tags[50]) {

    string temp_str{""};


    for (int i=0; i<50; i++) {
        if (strcmp(&tags[i], ",") == 0) {
            _tags.push_back(temp_str);
        } else if (strcmp(&tags[i], "Í")) {
            _tags.push_back(temp_str);
            return;
        } else {
            temp_str = temp_str + tags[i];
        }
    }
}

void Entities::RoomEntity::update(float delta_time) {
    Entity::update(delta_time);
}

void Entities::RoomEntity::destroy() {
    Entity::destroy();
}
