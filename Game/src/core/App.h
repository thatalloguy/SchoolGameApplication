//
// Created by allos on 11/19/2024.
//
#pragma once

#include "core/Window.h"

namespace Game {

    class App {

    public:
        App(int argc, char** argv);
        ~App();


        void run();

    private:
        Tyche::Window* _window;

    };

}

