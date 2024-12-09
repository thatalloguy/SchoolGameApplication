//
// Created by allos on 12/5/2024.
//

#ifndef CAMERA_SPEED
#define CAMERA_SPEED 300
#endif

#pragma once

#include "../tools/Tool.h"
#include "core/Window.h"
#include "renderer/TileRenderer.h"
#include "stl/vector.h"

namespace MapEditor {

    struct Room {
        vector<Tyche::Tile*> tiles;
    };


    class App {

    public:
        App() = default;
        ~App();

        void init();
        void run();

        Tyche::Tile& getCursor();
        Tyche::Window& getWindow();
        Tyche::Camera& getCamera();

        void placeTile(Vector2 pos, Tyche::Tile* tile);
        void removeTile(Vector2 pos);

    private:
        Tyche::Window _window{{{1280, 720}, "TombCrawler - Map Editor"}};
        Tyche::TileRenderer _tile_renderer{};
        Tyche::Camera _camera{{1280, 720}};

        vector<Tools::ToolInfo*> _tools;
        Tools::ToolInfo* _current_tool = nullptr;

        Room _current_room{};

        //default Debug Entities;
        Tyche::Tile _cursor{.scale = {30, 30}, .texture_pos = {3, 3}};

        void registerNewTool(Tools::ToolInfo* tool_info);
        void setCurrentTool(Tools::ToolInfo* tool_info);
        void checkForToolHotkeys();
        void initializeTools();
        void updateCurrentTool();
        void destroyAllTools();

        void initializeImGui();
        void deinitializeImGui();
        void newImGuiFrame();
        void renderImGuiFrame();

        void updateCamera(float frameTime);
    };

}
