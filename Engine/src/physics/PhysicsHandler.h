//
// Created by allos on 11/22/2024.
//
#pragma once

#include "math/vecs.h"
#include "PhysicsObject.h"
#include "stl/vector.h"

namespace Tyche {


    // x,y = min;
    // z, w = max;
    typedef Math::Vector4 AABB;

    struct PhysicsCollision {
        PhysicsObject* a{};
        PhysicsObject* b{};
        Math::Vector2 normal;
        Math::Vector2 correction;
    };

    namespace PhysicsHandler {


        bool isColliding(const AABB& a,const  AABB& b);
        float distance(const Math::Vector2& a, const Math::Vector2& b);

        void ResolveCollision(PhysicsObject& a, PhysicsObject& b);
        bool AABBvsAABB(PhysicsCollision& physics_collision);


    };

    class World {
    public:
        World(Math::Vector2 gravity = {0, 9.7f});
        ~World();

        PhysicsID addRigidBody(PhysicsObject* object);
        PhysicsID addStaticBody(PhysicsObject* object);

        void removeBody(PhysicsID id);

        void step(float delta_time);

        Math::Vector2 getGravity();
        void setGravity(const Math::Vector2& vec);

    private:
        STL::vector<PhysicsObject*> _bodies;

        unsigned int _id_count = 0;
        Math::Vector2 _gravity{0, 9.7f};


    };

}

