//
// Created by allos on 12/5/2024.
//
#pragma once
#include "../entities/Cursor.h"
#include "../tools/Tool.h"
#include "core/Window.h"
#include "renderer/EntityRenderer.h"
#include "stl/hashmap.h"
#include "stl/vector.h"

namespace MapEditor {


    class App {

    public:
        App() = default;
        ~App();

        void init();
        void run();

        MapEntities::Cursor& getCursorEntity();

    private:
        Tyche::Window _window{{{1280, 720}, "TombCrawler - Map Editor"}};
        Tyche::EntityRenderer _entity_renderer{};
        Tyche::Camera _camera{{1280, 720}};

        vector<Tools::ToolInfo*> _tools;
        Tools::ToolInfo* _current_tool = nullptr;

        //default Debug Entities;
        MapEntities::Cursor _cursor_entity{};

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
    };

}
