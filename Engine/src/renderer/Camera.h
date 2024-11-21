//
// Created by allos on 11/20/2024.
//
#pragma once

#include <math/mat4.h>

namespace Tyche {


    class Camera {

    public:
        //Just a simple constructor that just initializes internal values.
        Camera(const Math::Vector2& viewport_size);
        Camera() = default;
        ~Camera() = default;

        // Recalculates the internal camera matrix; and sets the viewport to the correct size
        void update();

        Math::Matrix4 getMatrix() const;

        void  setPosition(const Math::Vector2& new_position);
        Math::Vector2 getPosition()  const;

        Math::Vector2 getViewportSize() const;
        void  setViewportSize(const Math::Vector2& new_size);

    private:
        Math::Matrix4 _camera_matrix;
        Math::Vector2 _position;

        Math::Vector2 _viewport_size;
    };


}
