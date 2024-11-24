//
// Created by allos on 11/19/2024.
//
#pragma once

#include "core/Window.h"
#include "renderer/TileRenderer.h"
#include "renderer/EntityRenderer.h"
#include "physics/PhysicsObject.h"

namespace Game {

    class TestEntity: public Tyche::Entity {
    public:
        void test() {};
    };

    class App {

    public:
        App(int argc, char** argv);
        ~App();


        void run();

    private:
        Tyche::Window* _window;
        Tyche::TileRenderer _tile_renderer{};
        Tyche::EntityRenderer _entity_renderer{};
        Tyche::Camera _camera{};
        Tyche::PhysicsObject _object{};
        Tyche::PhysicsObject _object2{};
    };

}

