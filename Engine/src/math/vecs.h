//
// Created by allos on 11/19/2024.
//
#pragma once

namespace Tyche::Math {


    /*
     * This is a simple struct to represent a 2-dimensional vector.
     */
    struct Vector2 {

        // Very simple constructor.
        Vector2(float x, float y) {_[0] = x; _[1] = y; };

        // use an array instead of just float x, y;
        // so that for uniforms we can just point to the internal array for opengl.
        float _[2] = {0.0f, 0.0f};

        // we impl this operator since Vector2._[] looks very messy.
        float operator[](int index) const {
            return _[index];
        }

        // The value_ptr method is meant for opengl uniforms.
        float* value_ptr() {
            return _;
        }

        // Standard getters and setts
        float getX() const { return _[0]; };
        float getY() const { return _[1]; };

        void setX(float x) { _[0] = x; };
        void setY(float y) { _[1] = y; };


        // Implementations for my most used math operators.

        bool operator==(Vector2& b) const {
            return (_[0] == b[0] && _[1] == b[1]);
        }

        void operator+=(Vector2& b) {
            _[0] += b[0];
            _[1] += b[1];
        }

        void operator*=(Vector2& b) {
            _[0] *= b[0];
            _[1] *= b[1];
        }

        void operator*=(float b) {
            _[0] *= b;
            _[1] *= b;
        }

        void operator-=(Vector2& b) {
            _[0] -= b[0];
            _[1] -= b[1];
        }

        Vector2 operator+(Vector2& b) const {
            return {_[0] + b[0],_[1] + b[1]};
        }

        Vector2 operator-(Vector2& b) const {
            return {_[0] - b[0],_[1] - b[1]};
        }

    };

}