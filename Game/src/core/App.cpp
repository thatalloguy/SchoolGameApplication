//
// Created by allos on 11/19/2024.
//

#include <cstdlib>
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
        .position{200, 500},
        .texture_pos{1,1}
    };

    _tile_renderer.addTile(testTile);
    _tile_renderer.addTile(testTile2);

    Tyche::Input::setTargetWindow(*_window);

    Tyche::Input::addAction("test");
    Tyche::Input::addKey("test", Tyche::Input::Key::W);
    Tyche::Input::addKey("test", Tyche::Input::Key::D);

    //bob.setPosition({200, 400});
    _entity_renderer.addEntity(&bob);
    _object.setPosition(bob.getPosition());


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

        _object.step(frameTime, {0, 9.7f});
        bob.setPosition(_object.getPosition());




        _window->update();
        Tyche::Input::processInputs();

        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);

        if (Tyche::Input::isActionPressed("test")) {
            spdlog::info("key press");
        }



    }

}
