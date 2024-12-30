//
// Created by allos on 12/21/2024.
//
#pragma once


#include <physics/PhysicsObject.h>
#include "RoomEntity.h"
#include "../core/RoomManager.h"

namespace Entities {


    /*
     * The falling tile is a block that falls if the player touches it.
     * NOTE: the trigger to make the block fall is slightly larger than the actual entity size.
     * This is to give the player time to react to the change. It also allows the player to run over it when at max speed.
     */
class FallingTile: public Entities::RoomEntity {

    public:
        void initialize(const Vector2& position, Game::Room* room, char tags[50]) override;

        void update(float delta_time) override;
        void destroy() override;

        void reset() override;

    private:
        Vector4 _trigger;
        Vector2 _old_position{0, 0};
        Tyche::PhysicsObject _body{};

        bool is_falling = false;
        // Sleeping is a 1 frame delay to avoid falling again when the level resets.
        bool sleeping = false;

        Tyche::DebugRenderer* _debug_renderer = nullptr;
        Tyche::AudioEngine* _audio_engine   = nullptr;
        Game::RoomManager* _room_manager    = nullptr;
        Tyche::World* _world                = nullptr;
        Tyche::PhysicsObject* _player_body  = nullptr;
    };


}
