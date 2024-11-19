//
// Created by allos on 11/19/2024.
//

#include "App.h"

Game::App::App(int argc, char **argv) {
    Tyche::WindowCreationInfo defaultWindowInfo{
        .window_title = "Game"
    };

    _window = new Tyche::Window{defaultWindowInfo};

    _tile_renderer.initialize();

    Tyche::Tile testTile{};

    _tile_renderer.addTile(testTile);
}

Game::App::~App() {
    delete _window;
}

void Game::App::run() {

    while (!_window->shouldWindowClose()) {

        _window->update();

        _tile_renderer.renderTiles();
    }

}
