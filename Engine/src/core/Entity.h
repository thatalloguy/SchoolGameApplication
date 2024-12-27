//
// Created by allos on 11/20/2024.
//
#pragma once

#include "math/mat4.h"

namespace Tyche {


    /* Entities are dynamic gameobjects
     * Functions very simulair to tiles.
     * The Entity class is a base class, to create more entities please create new classes and inherit from this.
     */
    class Entity {
    public:
        Entity()  = default;
        virtual ~Entity() = default;

        virtual void initialize();
        virtual void update(float delta_time);
        virtual void prepareRendering();
        virtual void destroy();

        virtual void setPosition(const Math::Vector2& vec);
        void setScale(const Math::Vector2& vec);
        void setSprite(const Math::Vector2& sprite);


        Math::Vector2 getPosition();
        Math::Vector2 getScale();
        Math::Matrix4 getTransform();
        Math::Vector2 getSprite();

    protected:
        void calculateTransformMatrix();

        Math::Matrix4 _transform{};
        Math::Vector2 _position{100.0f, 100.0f};
        Math::Vector2 _scale{100.0f, 100.0f};
        Math::Vector2 _sprite{0.0f, 0.0f};

    };


}
