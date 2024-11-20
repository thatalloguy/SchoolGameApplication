//
// Created by allos on 11/19/2024.
//

#include "App.h"

Game::App::App(int argc, char **argv) {
    Tyche::WindowCreationInfo defaultWindowInfo{
        .window_title = "Game"
    };

    _window = new Tyche::Window{defaultWindowInfo};
    _camera.setViewportSize(_window->getWindowSize());

    Tyche::TileRendererCreationInfo tile_renderer_info {};
    _tile_renderer.initialize(tile_renderer_info);

    Tyche::Tile testTile{};
    Tyche::Tile testTile2{
        .position{200, 500},
        .texture_pos{1,1}
    };

    _tile_renderer.addTile(testTile);
    _tile_renderer.addTile(testTile2);

}

Game::App::~App() {
    delete _window;
}

void Game::App::run() {

    while (!_window->shouldWindowClose()) {

        _window->update();
        _camera.update();

        _tile_renderer.renderTiles(_camera);
    }

}
