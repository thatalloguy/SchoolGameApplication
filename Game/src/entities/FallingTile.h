//
// Created by allos on 12/21/2024.
//
#pragma once


#include <physics/PhysicsObject.h>
#include "RoomEntity.h"
#include "../core/RoomManager.h"

namespace Entities {

class FallingTile: public Entities::RoomEntity {

    public:
        void initialize(const Vector2& position, Game::Room* room, char tags[50]) override;

        void update(float delta_time) override;
        void destroy() override;

    private:
        Vector4 _trigger;
        Tyche::PhysicsObject _body{};
        Tyche::SoundID _falling_sound_id;

        bool is_falling = false;

        Tyche::DebugRenderer* _debug_renderer = nullptr;
        Tyche::AudioEngine* _audio_engine   = nullptr;
        Game::RoomManager* _room_manager    = nullptr;
        Game::Room* _room                   = nullptr;
        Tyche::World* _world                = nullptr;
        Tyche::PhysicsObject* _player_body  = nullptr;
    };


}
