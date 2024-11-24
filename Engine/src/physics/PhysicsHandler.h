//
// Created by allos on 11/22/2024.
//
#pragma once

#include "math/vecs.h"
#include "PhysicsObject.h"

namespace Tyche {


    // x,y = min;
    // z, w = max;
    typedef Math::Vector4 AABB;

    struct PhysicsCollision {
        PhysicsObject* a{};
        PhysicsObject* b{};
        Math::Vector2 normal;
    };

    namespace PhysicsHandler {


        bool isColliding(const AABB& a,const  AABB& b);
        float distance(const Math::Vector2& a, const Math::Vector2& b);

        void ResolveCollision(PhysicsObject& a, PhysicsObject& b);
        bool AABBvsAABB(PhysicsCollision& physics_collision);


    };

}

