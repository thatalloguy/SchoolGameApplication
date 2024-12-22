//
// Created by allos on 12/3/2024.
//
#pragma once
#include <core/Entity.h>

#include "physics/PhysicsObject.h"

namespace Entities {


    class Player : public Tyche::Entity {


    public:
        void initialize() override;
        void update(float delta_time) override;
        void destroy() override;

        Tyche::PhysicsObject& getBody();

    private:

        Tyche::PhysicsObject _collision_body;

    };



}
