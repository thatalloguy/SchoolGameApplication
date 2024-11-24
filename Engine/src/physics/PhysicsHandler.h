//
// Created by allos on 11/22/2024.
//
#pragma once

#include "math/vecs.h"

namespace Tyche {


    // x,y = min;
    // z, w = max;
    typedef Math::Vector4 AABB;

    class PhysicsHandler {

    public:

        static bool isColliding(const AABB& a,const  AABB& b);
        static float distance(const Math::Vector2& a, const Math::Vector2& b);

        void addPhysicsObject();
        void clearPhysicsObject();



    };

}

