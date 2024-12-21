//
// Created by allos on 12/19/2024.
//

#pragma once
#include <core/Entity.h>

#include "physics/PhysicsObject.h"

namespace Entities {


    class Torch : public Tyche::Entity {


    public:
        void initialize() override;
        void update() override;
        void destroy() override;


    };



}
