//
// Created by allos on 12/19/2024.
//

#pragma once
#include <core/Entity.h>

#include "physics/PhysicsObject.h"

namespace Entities {

    /*
     * Unused.
     * Was supposed to be an ambient entity that would just play a simple sound on loop.
     */

    class Torch : public Tyche::Entity {


    public:
        void initialize() override;
        void update(float delta_time) override;
        void destroy() override;


    };



}
