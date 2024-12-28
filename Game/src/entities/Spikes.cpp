//
// Created by allos on 12/28/2024.
//

#include "Spikes.h"

void Entities::Spikes::initialize(const Vector2 &position, Game::Room *room, char *tags) {
    RoomEntity::initialize(position, room, tags);

    _scale = {25, 25};
    _sprite = {1, 1};


    _trigger = {
            _position[0] - 25,
            _position[1] + 12.5f,
            _position[0] + 25,
            _position[1] + 15.5f
    };


    _room_manager = (Game::RoomManager*) room->parent;
    _world = _room_manager->getWorld();

    _player_body = _world->getFirstBody();
    _debug_renderer = _room_manager->getDebugRenderer();
}

void Entities::Spikes::update(float delta_time) {
    RoomEntity::update(delta_time);

    _debug_renderer->renderBox(_trigger);

    if (Tyche::PhysicsHandler::collision(_trigger, _player_body->getAABB())
        && _state == SpikeState::IN) {
        _delay_timer.start([=, this](){
            this->activate();
        });
    }

    if (_state == SpikeState::IN) {
        _sprite = {1, 1};

        _trigger = {
                _position[0] - 25,
                _position[1] + 12.5f,
                _position[0] + 25,
                _position[1] + 15.5f
        };
    } else {
        _sprite = {2, 1};

        _trigger = {
                _position[0] - 25,
                _position[1] - 25,
                _position[0] + 25,
                _position[1] + 25
        };
    }
}

void Entities::Spikes::destroy() {
    RoomEntity::destroy();
}

void Entities::Spikes::activate() {
    _state = SpikeState::OUT;

    _delay_timer.setWaitTime(reactivation_delay);

    _delay_timer.start([=, this](){
       _state = SpikeState::IN;
       _delay_timer.setWaitTime(activation_delay);
    });

}

void Entities::Spikes::reset() {
    RoomEntity::reset();

    _delay_timer.stop();
    _state = SpikeState::IN;
}
