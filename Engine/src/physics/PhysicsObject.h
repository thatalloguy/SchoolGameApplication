//
// Created by allos on 11/24/2024.
//

#pragma once

#include "PhysicsHandler.h"

namespace Tyche {

    class PhysicsObject {

    public:
        PhysicsObject() = default;
        PhysicsObject(const Math::Vector2& position, const Math::Vector2& init_vel, float mass = 1.0f); //init_vel = Initial Velocity
        ~PhysicsObject() = default;

        void step(float delta_time, Math::Vector2 gravity);

        Math::Vector2 getPosition();
        Math::Vector2 getVelocity();
        Math::Vector2 getForce();
        Math::Vector4 getAABB();
        float getMass();

        void setPosition(const Math::Vector2& position);
        void setVelocity(const Math::Vector2& velocity);
        void setForce(const Math::Vector2& force);
        void setMass(float mass);
        void setAABB(const Math::Vector4& AABB);

    private:

        Math::Vector2 _position{0, 0};
        Math::Vector2 _velocity{0, 0};
        Math::Vector2 _force{0, 0};
        float _mass = 1.0f;

        Math::Vector4 _aabb{0.0f, 0.0f, 0.0f, 0.0f};
    };


}
