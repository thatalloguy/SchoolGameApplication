//
// Created by allos on 11/19/2024.
//
#pragma once

#include "vecs.h"

namespace Tyche::Math {


    //Simple Struct to represent a 4 by 4 float matrix
    struct Matrix4 {

        float _[4][4] = {
                {1.f, 0.f, 0.f, 0.f},
                {0.f, 1.f, 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f}
        };

        //Returns the array of the given row (index)
        float* operator[](int index) {
            return _[index];
        }

        // translates the Matrix according to the Vector2
        void translate(const Vector2& vec) {
            // translate the x dimension
            Vector4 m1 = {_[0][0] * vec[0], _[0][1] * vec[0], _[0][1] * vec[0], _[0][3] * vec[0]};
            // translate the y dimension
            Vector4 m2 = {_[1][0] * vec[0], _[1][1] * vec[1], _[1][2] * vec[1], _[1][3] * vec[1]};
            //Keep the Z dimension the same since its a 2d game.
            Vector4 m3 = {_[3][0], _[3][1], _[3][2], _[3][3]};

            //Calc the result
            Vector4 result = m1 + m2 + m3;

            //Apply the result to our internal data.
            _[3][0] = result[0];
            _[3][1] = result[1];
            _[3][2] = result[2];
            _[3][3] = result[3];
        }

        // Scales the Matrix according to the Vector2
        void scale(const Vector2& vec) {

            //Scale the X dimension
            Vector4 m1 = {_[0][0] * vec[0], _[0][1] * vec[0], _[0][1] * vec[0], _[0][3] * vec[0]};
            //Scale the Y dimension
            Vector4 m2 = {_[1][0] * vec[0], _[1][1] * vec[1], _[1][2] * vec[1], _[1][3] * vec[1]};

            //Apply our results to the internal buffer.
            _[0][0] = m1[0];
            _[0][1] = m1[1];
            _[0][2] = m1[2];
            _[0][3] = m1[3];

            _[1][0] = m2[0];
            _[1][1] = m2[1];
            _[1][2] = m2[2];
            _[1][3] = m2[3];

        }

        void ortho(float left, float right, float bottom, float top, float near, float far) {
            _[0][0] = 2 / (right - left);
            _[1][1] = 2 / (top - bottom);
            _[2][2] = - 2 / (far - near);

            _[3][0] = - (right + left) / (right - left);
            _[3][1] = - (top + bottom) / (top - bottom);
            _[3][2] =  (far + near) / (far - near);
        }

        //Returns the begin of the internal buffer (basically [0][0]).
        //Meant for opengl uniforms
        float* value_ptr() {
            return _[0];
        }
    };

}