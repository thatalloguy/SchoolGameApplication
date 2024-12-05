//
// Created by allos on 12/5/2024.
//
#pragma once

#ifndef GRID_SIZE
#define GRID_SIZE 200
#endif

#include "core/Input.h"
#include "stl/string.h"

namespace MapEditor {
    class App;
}

namespace Tools {

    class Tool {

    public:

        virtual void initialize(MapEditor::App& editor_instance) {};
        virtual void update() {};
        virtual void onSwitch() {};
        virtual void destroy() {};

    };

    enum class ToolType: unsigned int {
        MouseTool = 0,
        GeneralTool = 2
    };

    struct ToolInfo {
        Tyche::STL::string name = "Tool";
        ToolType type = ToolType::GeneralTool;
        Tyche::Input::Key hotkey = Tyche::Input::E;

        Tool* instance = nullptr;
    };
}
