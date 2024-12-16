//
// Created by allos on 11/19/2024.
//
#pragma once

#include "RoomManager.h"
#include "core/Window.h"
#include "renderer/TileRenderer.h"
#include "renderer/EntityRenderer.h"
#include "physics/PhysicsHandler.h"
#include "audio/AudioDevice.h"

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

        void processArgs(int argc, char** argv);
        void debugRoom(const char* path);

        Tyche::Window* _window;
        Tyche::TileRenderer _tile_renderer{};
        Tyche::EntityRenderer _entity_renderer{};
        Tyche::DebugRenderer* _debug_renderer = nullptr;

        Tyche::AudioEngine _audio_engine;

        Tyche::Camera _camera{};

        Tyche::World* _world{};
        RoomManager* _room_manager = nullptr;
    };

}

