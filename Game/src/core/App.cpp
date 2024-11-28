//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "core/Input.h"

namespace {
    Tyche::Entity bob{};
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


    Tyche::Tile testTile{};
    Tyche::Tile testTile2{
        .position{100, 700},
        .texture_pos{1,1}
    };

    _tile_renderer.addTile(testTile);
    _tile_renderer.addTile(testTile2);

    Tyche::Input::setTargetWindow(*_window);

    Tyche::Input::addAction("jump");
    Tyche::Input::addAction("left");
    Tyche::Input::addAction("right");
    Tyche::Input::addKey("jump", Tyche::Input::Key::W);
    Tyche::Input::addKey("left", Tyche::Input::Key::D);
    Tyche::Input::addKey("right", Tyche::Input::Key::A);

    //bob.setPosition({200, 400});
    _entity_renderer.addEntity(&bob);
    _object.setPosition({100, 0});
    _object.setAABB(200, 200);

    _object2.setPosition({100, 700});
    _object2.setAABB(200, 200);


    _object3.setPosition({500, 500});
    _object3.setAABB(200, 200);


    _world.addRigidBody(&_object);
    _world.addStaticBody(&_object2);
    _world.addStaticBody(&_object3);

}

Game::App::~App() {
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

        if (Tyche::Input::isActionPressed("jump")) {
            _object.setVelocity(Tyche::Math::Vector2{_object.getVelocity().getX(), -4000} * frameTime);
        }

        if (Tyche::Input::isActionPressed("left")) {
            _object.setVelocity(Tyche::Math::Vector2{4000, _object.getVelocity().getY()} * frameTime);
        }
        if (Tyche::Input::isActionPressed("right")) {
            _object.setVelocity(Tyche::Math::Vector2{-4000, _object.getVelocity().getY()} * frameTime);
        }


       _world.step(frameTime);


        bob.setPosition(_object.getPosition());


        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);




    }

}
