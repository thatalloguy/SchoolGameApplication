//
// Created by allos on 12/30/2024.
//

#include "Spring.h"

//global constants
namespace {
    unsigned int spring_sound = -1;
}

void Entities::Spring::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);


    _scale = {25, 25};
    _sprite = {3, 1};


    _trigger = {
            _position[0] - 15,
            _position[1] + 12.5f,
            _position[0] + 15,
            _position[1] + 15.5f
    };


    _room_manager = (Game::RoomManager*) room->parent;
    _world = _room_manager->getWorld();

    _player_body = _world->getFirstBody();
    _debug_renderer = _room_manager->getDebugRenderer();
    _audio_engine = _room_manager->getAudioEngine();

    if (spring_sound == -1) {
        spring_sound = _audio_engine->registerSound({"../../../Resources/Audio/spring.wav"});
    }
}

void Entities::Spring::update(float delta_time) {
    RoomEntity::update(delta_time);


    _debug_renderer->renderBox(_trigger);


    // Launch the player upwards when it comes in contact with us.
    if (Tyche::PhysicsHandler::collision(_trigger, _player_body->getAABB()) && _state == SpringState::IN) {
        _state = SpringState::OUT;

        _audio_engine->playSound(spring_sound, _position, 2.0f);

        // Set a delay to go back to IN state
        _delay_timer.setWaitTime(turn_off_delay);
        _delay_timer.start([=, this](){
            _state = SpringState::IN;
        });

        // Slightly slow down the player on the x-axis.
        _player_body->setVelocity({_player_body->getVelocity()[0] * 0.995f, -100});
    }


    if (_state == SpringState::IN) {
        _sprite = {3, 1};
    } else {
        _sprite = {0, 2};

        _audio_engine->updateSound(spring_sound, _position);
    }
}

void Entities::Spring::destroy() {
    RoomEntity::destroy();
}

void Entities::Spring::reset() {
    RoomEntity::reset();
}

void Entities::Spring::activate() {

}
