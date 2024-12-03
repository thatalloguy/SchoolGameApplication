//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "../entities/Player.h"
#include "core/Input.h"

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

    _object2.setPosition({100, 700});
    _object2.setAABB(1200, 200);



    _world.addRigidBody(&player.getBody());
    _world.addStaticBody(&_object2);

    _audio_engine.Init();

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

       _world.step(frameTime);


        player.update();


        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);




    }

}
