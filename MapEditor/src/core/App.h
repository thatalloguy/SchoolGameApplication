//
// Created by allos on 12/5/2024.
//
#pragma once
#include "../tools/Tool.h"
#include "core/Window.h"
#include "stl/hashmap.h"
#include "stl/vector.h"

namespace MapEditor {


    class App {

    public:
        App() = default;
        ~App();

        void init();
        void run();

    private:
        Tyche::Window _window{{{1280, 720}, "TombCrawler - Map Editor"}};

        vector<Tools::ToolInfo*> _tools;
        Tools::ToolInfo* _current_tool = nullptr;

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
