//
// Created by allos on 12/30/2024.
//
#pragma once

#include "RoomEntity.h"
#include "../core/RoomManager.h"
#include "core/Timer.h"

namespace Entities {


    enum class SpringState : unsigned int {
        IN = 0,
        OUT = 1,
    };

    /*
     * The spring entity is very similar to the spike entity except it launches the player up.
     */

    class Spring: public RoomEntity {
    public:

        void initialize(const Vector2 &position, Game::Room *room, char tags[50]) override;

        void update(float delta_time) override;

        void destroy() override;

        SpringState getState() { return _state; };

        void reset() override;

        void activate();

    private:

        const int turn_off_delay = 400;

        SpringState _state = SpringState::IN;
        Vector4 _trigger;
        Tyche::Timer _delay_timer;

        Tyche::DebugRenderer* _debug_renderer   = nullptr;
        Game::RoomManager* _room_manager        = nullptr;
        Tyche::World* _world                    = nullptr;
        Tyche::PhysicsObject* _player_body      = nullptr;

    };


}

