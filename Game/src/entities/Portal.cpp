//
// Created by allos on 12/22/2024.
//

#include "Portal.h"

void Entities::Portal::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _sprite = {1, 0};
    _scale = {0, 0};


    if (RoomEntity::hasTag("end")) {
        _type = PortalType::END;
    }

    _room_manager = (Game::RoomManager*) room->parent;
    _world = _room_manager->getWorld();

    _player_body = _world->getFirstBody();
    _debug_renderer = _room_manager->getDebugRenderer();

}

void Entities::Portal::update(float delta_time) {
    RoomEntity::update(delta_time);

    _debug_renderer->renderBox({_position[0] - 12.5f, _position[1] - 12.5f,
                                      _position[0] + 12.5f, _position[1] + 12.5f});
}

void Entities::Portal::destroy() {
    RoomEntity::destroy();
}
