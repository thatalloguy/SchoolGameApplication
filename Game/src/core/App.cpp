//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "audio/AudioDevice.h"
#include "core/Input.h"

namespace {
    Tyche::Entity bob{};
    Tyche::Entity bob2{};
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
    _entity_renderer.addEntity(&bob2);
    _object.setPosition({100, 0});
    _object.setAABB(200, 200);

    _object2.setPosition({100, 700});
    _object2.setAABB(200, 200);


    _object3.setPosition({500, 500});
    _object3.setAABB(200, 200);


    _object4.setPosition({500, 0});
    _object4.setAABB(200, 200);


    _world.addRigidBody(&_object);
    _world.addRigidBody(&_object4);
    _world.addStaticBody(&_object2);
    _world.addStaticBody(&_object3);

    Tyche::AudioDevice::initializeDevice();
}

Game::App::~App() {
    Tyche::AudioDevice::cleanUp();
    delete _window;
}

void Game::App::run() {
    auto currentTime = std::chrono::high_resolution_clock ::now();

    //auto sound = Tyche::AudioDevice::loadSound("../../../Resources/Audio/bluebonnet_in_b_major_looped.wav");

//    Tyche::AudioDevice::playSound(sound, {0, 0});

    while (!_window->shouldWindowClose()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        double frameTime = (double) std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count() / 100000;
        currentTime = newTime;

        // If we dont cap it, then the physics engine will explode since instead of correcting the velocities we are enlarging them.
        if (frameTime >= 0.166)
            frameTime = 0.166;


        _window->update();

        if (Tyche::Input::isActionPressed("jump")) {
            _object.setVelocity(Tyche::Math::Vector2{_object.getVelocity().getX(), -40});
        }

        if (Tyche::Input::isActionPressed("left")) {
            _object.setVelocity(Tyche::Math::Vector2{40, _object.getVelocity().getY()});
        }
        if (Tyche::Input::isActionPressed("right")) {
            _object.setVelocity(Tyche::Math::Vector2{-40, _object.getVelocity().getY()});
        }


       _world.step(frameTime);


        bob.setPosition(_object.getPosition());
        bob2.setPosition(_object4.getPosition());


        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);




    }

}
