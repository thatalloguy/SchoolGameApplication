//
// Created by allos on 12/22/2024.
//

#include "Portal.h"

//global constants
namespace {
    unsigned int portal_sound = -1;
}

void Entities::Portal::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _sprite = {0, 1};
    _scale = {25, 25};


    if (RoomEntity::hasTag("end")) {
        _type = PortalType::END;
    }

    _trigger = {_position[0] - 12.5f, _position[1] - 12.5f,
                _position[0] + 12.5f, _position[1] + 12.5f};

    _room_manager = (Game::RoomManager*) room->parent;
    _world = _room_manager->getWorld();

    _player_body = _world->getFirstBody();
    _debug_renderer = _room_manager->getDebugRenderer();
    _audio_engine = _room_manager->getAudioEngine();


    if (portal_sound == -1) {
        portal_sound = _audio_engine->registerSound({"../../../Resources/Audio/level.wav"});
    }
}

void Entities::Portal::update(float delta_time) {
    RoomEntity::update(delta_time);

    _debug_renderer->renderBox(_trigger);

    if (Tyche::PhysicsHandler::collision(_trigger, _player_body->getAABB())
            && _type == PortalType::END) {
        _room_manager->goToNextRoom();
    }
}

void Entities::Portal::destroy() {
    RoomEntity::destroy();
}
