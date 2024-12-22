//
// Created by allos on 12/12/2024.
//
#pragma once


#include <core/typedefs.h>
#include <math/vecs.h>

#include "Camera.h"
#include "Shader.h"
#include "stl/vector.h"

using namespace Tyche::Math;

namespace Tyche {

    class DrawCommand {
    public:
        virtual void render(Shader& shader, Camera& camera) {};
    };


    class LineDrawCommand: public DrawCommand {

    public:
        LineDrawCommand(const Vector2& begin, const Vector2& end);
        ~LineDrawCommand() = default;

        void render(Shader& shader, Camera& camera) override;
    private:
        Vector2 _begin;
        Vector2 _end;
    };

    class PointDrawCommand: public DrawCommand {
    public:
        PointDrawCommand(const Vector2& position);
        ~PointDrawCommand() = default;

        void render(Shader& shader, Camera& camera) override;
    private:
        Matrix4 _transform;
    };


    class DebugRenderer {

    public:
        DebugRenderer();
        ~DebugRenderer();


        void renderLine(const Vector2& begin, const Vector2& end);
        void renderPoint(const Vector2& position);
        void renderBox(const Vector4& AABB);

        void render(Camera& camera);
        void clearQueue();

    private:

        STL::vector<DrawCommand*> _queue;
        Shader _debug_shader;


    };

}