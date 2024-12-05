//
// Created by allos on 12/5/2024.
//
#pragma once
#include "core/Window.h"

namespace MapEditor {


    class App {

    public:
        App() = default;
        ~App();

        void init();
        void run();

    private:
        Tyche::Window _window{{{1280, 720}, "TombCrawler - Map Editor"}};


        void initializeImGui();
        void deinitializeImGui();
        void newImGuiFrame();
        void renderImGuiFrame();
    };

}
