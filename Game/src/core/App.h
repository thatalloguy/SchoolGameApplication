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

    enum class GameState : unsigned int {
        PAUSED = 0,
        GAMELOOP = 1,
        START_MENU = 2
    };

    class App {

    public:
        App(int argc, char** argv);
        ~App();


        void run();

    private:

        void processArgs(int argc, char** argv);
        void debugRoom(const char* path);

        void mainloopTick(float frameTime);
        void menuTick(float frameTime);

        void initializeImgui();
        void cleanupImgui();

        void newImguiFrame();
        void renderImguiFrame();


        Tyche::Window* _window;
        Tyche::TileRenderer _tile_renderer{};
        Tyche::EntityRenderer _entity_renderer{};
        Tyche::DebugRenderer* _debug_renderer = nullptr;

        GameState _current_state = GameState::START_MENU;

        Tyche::AudioEngine _audio_engine;

        Tyche::Camera _camera{};

        Tyche::World* _world{};
        RoomManager* _room_manager = nullptr;

        bool _debug_rendering = false;
    };

}

