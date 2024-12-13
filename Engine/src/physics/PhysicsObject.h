//
// Created by allos on 11/24/2024.
//

#pragma once

#include "math/vecs.h"

namespace Tyche {


    typedef unsigned int PhysicsID;

    enum class ObjectType: unsigned int {
        Static = 0,
        Rigid = 1
    };

    struct CollisionInfo {
        bool is_colliding = false;
        Math::Vector2 collision_normal{0, 0};
    };

    // A physics object represents a physical object.
    class PhysicsObject {

    public:
        PhysicsObject() = default;
        PhysicsObject(const Math::Vector2& position, const Math::Vector2& init_vel, float mass = 1.0f); //init_vel = Initial Velocity
        PhysicsObject(Math::Vector4& AABB, ObjectType type);

        ~PhysicsObject() = default;

        // Applies gravity to our velocity and updates our position.
        void step(float delta_time, Math::Vector2 gravity);

        // Getters and setters
        Math::Vector2 getPosition();
        Math::Vector2 getVelocity();
        Math::Vector2 getForce();
        Math::Vector4 getAABB();
        float getMass();
        float getRestitution();
        PhysicsID getID();
        ObjectType getObjectType();

        void setPosition(const Math::Vector2& position);
        void setVelocity(const Math::Vector2& velocity);
        void setForce(const Math::Vector2& force);
        void setMass(float mass);
        void setRestitution(float restitution);
        void setAABB(float w, float h);
        void setID(PhysicsID id);
        void setObjectType(ObjectType type);

        void addForce(const Math::Vector2& force);
        void setCorrection(const Math::Vector2& cor);

        CollisionInfo getCollisionInfo();
        void setCollisionInfo(const CollisionInfo& new_info);
    private:

        PhysicsID  _id = 0;
        ObjectType _type = ObjectType::Static;
        CollisionInfo _col_info{};

        Math::Vector2 _position{0, 0};
        Math::Vector2 _correction{0, 0};
        Math::Vector2 _velocity{0, 0};
        Math::Vector2 _force{0, 0};

        float _mass = 1.0f;
        float _restitution = 0.1f;

        Math::Vector4 _aabb{0.0f, 0.0f, 0.0f, 0.0f};
        Math::Vector2 _size{0.0f, 0.0f};
    };


}
