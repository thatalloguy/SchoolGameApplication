//
// Created by allos on 11/19/2024.
//

#include "App.h"

#include "core/Input.h"

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

    TestEntity bob{};
    _entity_renderer.addEntity(bob);

    Tyche::Input::setTargetWindow(*_window);

    Tyche::Input::addAction("test");
    Tyche::Input::addKey("test", Tyche::Input::Key::W);
    Tyche::Input::addKey("test", Tyche::Input::Key::D);

}

Game::App::~App() {
    delete _window;
}

void Game::App::run() {

    while (!_window->shouldWindowClose()) {

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
