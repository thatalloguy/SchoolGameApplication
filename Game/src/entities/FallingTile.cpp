//
// Created by allos on 12/21/2024.
//

#include "FallingTile.h"
#include "spdlog/spdlog.h"


namespace {
    Tyche::SoundID _falling_sound_id = -1;
}

void Entities::FallingTile::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _sprite = {3, 0};
    _scale = {25, 25};
    _position = position;
    _old_position = position;

    //The collider is above the block
     _trigger = {
        _position.getX() - 30,
        _position.getY() - 20,
        _position.getX() + 30,
        _position.getY() + 20
    };

     _body.setAABB({
                           _position.getX() - 25,
                           _position.getY() - 2.0f,
                           _position.getX() + 12.5f,
                           _position.getY() + 40.0f
                   });
     _body.setPosition(_position);



    _room_manager = (Game::RoomManager*) room->parent;
    _world = _room_manager->getWorld();
    _room = room;
    _audio_engine = _room_manager->getAudioEngine();

    _player_body = _world->getFirstBody();
    _debug_renderer = _room_manager->getDebugRenderer();

    _world->addStaticBody(&_body);
    if (_falling_sound_id == -1) {
        _falling_sound_id = _audio_engine->registerSound({"../../../Resources/Audio/falling_block.wav"});
    }
}


void Entities::FallingTile::update(float delta_time) {

    _debug_renderer->renderBox(_trigger);

    if (Tyche::PhysicsHandler::collision(_trigger, _player_body->getAABB()) && !is_falling && !sleeping) {
        is_falling = true;
        _audio_engine->playSound(_falling_sound_id, _position);
    }

    if (is_falling && !sleeping) {
        _body.step(delta_time, _world->getGravity());

        _position = _body.getPosition();

        _body.setAABB({
                              _position.getX() - 25,
                              _position.getY() - 2.0f,
                              _position.getX() + 12.5f,
                              _position.getY() + 40.0f
                      });

        _audio_engine->updateSound(_falling_sound_id, _position);
    }


    sleeping = false;
}

void Entities::FallingTile::destroy() {
}

void Entities::FallingTile::reset() {
    is_falling = false;
    sleeping = true;
    _position = _old_position;

    _body.setAABB({
                          _position.getX() - 25,
                          _position.getY() - 2.0f,
                          _position.getX() + 12.5f,
                          _position.getY() + 40.0f
                  });


    _body.setPosition(_position);
    _body.setVelocity({0, 0});

}
