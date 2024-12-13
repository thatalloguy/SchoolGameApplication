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


    // Some general Physics functions
    namespace PhysicsHandler {

        // Returns wether or not the boxes are colliding
        bool collision(const AABB& a,const  AABB& b);

        // Calculates / adjustes the velocities of the objects (and correction positions if needed)
        void resolveCollision(Tyche::PhysicsObject &a, Tyche::PhysicsObject &b);

        // Returns the correct correction position
        // aCenter = the Center of box A
        // bCenter = the Center of box B
        Math::Vector2 getCorrection(const Math::Vector2& aCenter, const Math::Vector2& bCenter, const AABB& A, const AABB& B);


    };

    // The world is a class the manages and updates PhysicsBodies
    class World {
    public:
        // The default gravity of earth is 9.7N
        World(bool handle_memory=false, Math::Vector2 gravity = {0, 9.7f});
        ~World();

        PhysicsID addRigidBody(PhysicsObject* object);
        PhysicsID addStaticBody(PhysicsObject* object);

        void removeBody(PhysicsID id);

        // Updates all of the bodies in the world
        void step(float delta_time);

        // getters and setters
        Math::Vector2 getGravity();
        void setGravity(const Math::Vector2& vec);

    private:
        STL::vector<PhysicsObject*> _bodies;

        unsigned int _id_count = 0;
        Math::Vector2 _gravity{0, 9.7f};

        bool _should_handle_memory = false;


    };

}

