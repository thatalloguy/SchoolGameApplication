//
// Created by allos on 12/3/2024.
//
#pragma once
#include <core/Entity.h>

#include "physics/PhysicsObject.h"

namespace Entities {

    // The player is the main entity and only controlled by that player :D
    class Player : public Tyche::Entity {


    public:
        void initialize() override;
        void update(float delta_time) override;
        void destroy() override;

        void setPosition(const Tyche::Math::Vector2& pos) override;

        Tyche::PhysicsObject& getBody();

    private:

        Tyche::PhysicsObject _collision_body;

    };



}
