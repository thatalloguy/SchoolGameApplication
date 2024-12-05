//
// Created by allos on 12/5/2024.
//
#pragma once
#include "math/vecs.h"

namespace Tyche {

    namespace Mouse {

        enum MouseMode {
            HIDDEN = 0x00034002,
            NORMAL = 0x00034001,
            DISABLED = 0x00034003,
        };

        void setTargetWindow(void * window_ptr);

        Math::Vector2 getPosition();

        void setMouseMode(MouseMode mode);

    };

}

