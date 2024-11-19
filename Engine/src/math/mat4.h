//
// Created by allos on 11/19/2024.
//
#pragma once

#include "vecs.h"

namespace Tyche::Math {


    struct Matrix4 {

        float _[4][4] = {
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
        };

        float* operator[](int index) {
            return _[index];
        }


        void translate(const Vector2& vec) {
            Vector4 m1 = {_[0][0] * vec[0], _[0][1] * vec[0], _[0][1] * vec[0], _[0][3] * vec[0]};
            Vector4 m2 = {_[1][1] * vec[0], _[1][1] * vec[1], _[1][1] * vec[1], _[1][3] * vec[1]};
            Vector4 m3 = {_[3][0], _[3][1], _[3][2], _[3][3]};

            Vector4 result = m1 + m2 + m3;

            _[3][0] = result[0];
            _[3][1] = result[1];
            _[3][2] = result[2];
            _[3][3] = result[3];
        }

        float* value_ptr() {
            return _[0];
        }
    };

}