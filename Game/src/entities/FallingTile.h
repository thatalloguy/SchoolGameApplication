//
// Created by allos on 12/21/2024.
//
#pragma once


#include <physics/PhysicsObject.h>
#include "RoomEntity.h"

namespace Entities {

class FallingTile: public Entities::RoomEntity {

    public:
        void initialize(const Vector2& position, Game::Room* room, char tags[50]) override;

        void update() override;
        void destroy() override;

    private:

        Tyche::PhysicsObject _body{};
    };


}
