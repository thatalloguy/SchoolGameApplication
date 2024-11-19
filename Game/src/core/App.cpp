//
// Created by allos on 11/19/2024.
//

#include "App.h"

Game::App::App(int argc, char **argv) {
    Tyche::WindowCreationInfo defaultWindowInfo{
        .window_title = "Game"
    };

    _window = new Tyche::Window{defaultWindowInfo};
}

Game::App::~App() {
    delete _window;
}

void Game::App::run() {

    while (!_window->shouldWindowClose()) {

        _window->update();
    }

}
