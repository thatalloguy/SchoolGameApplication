//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "../entities/Player.h"
#include "../entities/FallingTile.h"
#include "core/Input.h"

//Global constants
namespace {
    Entities::Player player;
}

Game::App::App(int argc, char **argv) {



    Tyche::WindowCreationInfo defaultWindowInfo{
        .window_title = "Game"
    };

    _window = new Tyche::Window{defaultWindowInfo};
    _camera.setViewportSize(_window->getWindowSize());

    Tyche::TileRendererCreationInfo tile_renderer_info {};
    _tile_renderer.initialize(tile_renderer_info);

    Tyche::EntityRendererCreationInfo entity_renderer_info {};
    _entity_renderer.initialize(entity_renderer_info);

    _debug_renderer = new Tyche::DebugRenderer{};


    Tyche::Tile floor{
        .position{100, 700},
        .scale{1200, 100},
        .texture_pos{0, 0}
    };

    _tile_renderer.addTile(floor);

    Tyche::Input::setTargetWindow(*_window);

    Tyche::Input::addAction("jump");
    Tyche::Input::addAction("left");
    Tyche::Input::addAction("right");
    Tyche::Input::addKey("jump", Tyche::Input::Key::W);
    Tyche::Input::addKey("left", Tyche::Input::Key::D);
    Tyche::Input::addKey("right", Tyche::Input::Key::A);

    player.initialize();

    _entity_renderer.addEntity(&player);

    _room_manager = new RoomManager(_tile_renderer, _entity_renderer, *_debug_renderer);

    _room_manager->registerEntity<Entities::FallingTile>(0);

    _world = _room_manager->getWorld();

    _world->addRigidBody(&player.getBody());

    _room_manager->loadRoom("../../../Rooms/begin.room");

    _audio_engine.Init();


    processArgs(argc, argv);
}

Game::App::~App() {
    _audio_engine.CleanUp();
    delete _window;
}

void Game::App::run() {
    auto currentTime = std::chrono::high_resolution_clock ::now();


    while (!_window->shouldWindowClose()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        double frameTime = (double) std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count() / 100000;
        currentTime = newTime;

        // If we dont cap it, then the physics engine will explode since instead of correcting the velocities we are enlarging them.
        if (frameTime >= 0.166)
            frameTime = 0.166;


        _window->update();

        _world->step(frameTime);
        _world->renderBodies(*_debug_renderer);

        _room_manager->update(frameTime);

        player.update(frameTime);


        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);

        if (_debug_rendering) {
            _debug_renderer->render(_camera);
        }

        _debug_renderer->clearQueue();




    }


    delete _debug_renderer;
}

void Game::App::processArgs(int argc, char** argv) {
    for (int i=0; i< argc; i++) {
        if (strcmp("-d", argv[i]) == 0) {
            debugRoom(argv[i + 1]);
        }
    }
}

void Game::App::debugRoom(const char* path) {
    spdlog::info("Path {}", path);
}
