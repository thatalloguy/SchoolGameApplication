//
// Created by allos on 12/22/2024.
//

#include "Portal.h"

void Entities::Portal::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _sprite = {0, 1};

    if (RoomEntity::hasTag("end")) {
        _type = PortalType::START;
    }
}

void Entities::Portal::update(float delta_time) {
    RoomEntity::update(delta_time);
}

void Entities::Portal::destroy() {
    RoomEntity::destroy();
}
