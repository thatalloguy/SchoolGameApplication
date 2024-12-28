//
// Created by allos on 12/28/2024.
//
#pragma once

#include "RoomEntity.h"
#include "../core/RoomManager.h"
#include "core/Timer.h"

namespace Entities {

    enum class SpikeState: unsigned int{
        IN = 0,
        OUT = 1,
    };

    class Spikes : public RoomEntity {
    public:

        void initialize(const Vector2& position, Game::Room* room, char tags[50]) override;

        void update(float delta_time) override;
        void destroy() override;

        SpikeState getState() { return _state; };

        void reset() override;

        void activate();
    private:
        const int activation_delay = 400;
        const int reactivation_delay = 2000;

        Vector4 _trigger;
        Tyche::Timer _delay_timer{activation_delay};
        SpikeState _state = SpikeState::IN;

        Tyche::DebugRenderer* _debug_renderer   = nullptr;
        Game::RoomManager* _room_manager        = nullptr;
        Tyche::World* _world                    = nullptr;
        Tyche::PhysicsObject* _player_body      = nullptr;

    };

}
