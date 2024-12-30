//
// Created by allos on 12/22/2024.
//
#pragma once

#include "RoomEntity.h"
#include "../core/RoomManager.h"

namespace Entities {

    enum class PortalType: unsigned int {
        START = 0,
        END = 1,
    };

    /*
     * The portal entity defines the length of a level.
     * Its either the start or the end of a level
     * When its the start of a level the player will get teleported to it when the level resets.
     * When its an end of a level, when it comes in contact with the player
     * it will tell the room manager to go to the next level.
     */

    class Portal: public RoomEntity {
    public:

        void initialize(const Vector2& position, Game::Room* room, char tags[50]) override;

        void update(float delta_time) override;
        void destroy() override;

        PortalType getType() { return _type; };

    private:
        PortalType _type = PortalType::START;

        Vector4 _trigger;

        Tyche::DebugRenderer* _debug_renderer   = nullptr;
        Game::RoomManager* _room_manager        = nullptr;
        Tyche::World* _world                    = nullptr;
        Tyche::PhysicsObject* _player_body      = nullptr;
        Tyche::AudioEngine* _audio_engine   = nullptr;
    };

}
