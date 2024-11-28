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
    };

    namespace PhysicsHandler {


        bool collision(const AABB& a,const  AABB& b);
        float distance(const Math::Vector2& a, const Math::Vector2& b);

        void resolveCollision(Tyche::PhysicsObject &a, Tyche::PhysicsObject &b);
        Math::Vector2 getCorrection(const Math::Vector2& aCenter, const Math::Vector2& bCenter, const AABB& A, const AABB& B);


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

