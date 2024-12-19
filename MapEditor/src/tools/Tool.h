//
// Created by allos on 12/5/2024.
//
#pragma once

#ifndef GRID_SIZE
#define GRID_SIZE 50
#endif

#include "core/Input.h"
#include "stl/string.h"

namespace MapEditor {
    class App;
}

namespace Tools {

    // A tool is a simple virtual class
    // Tools are for doing logic inside of the editor
    class Tool {

    public:

        virtual void initialize(MapEditor::App& editor_instance) {};
        virtual void update() {};
        virtual void onSwitch() {};
        virtual void destroy() {};

    };

    // Basic struct with everything about a Tool that we need.
    struct ToolInfo {
        Tyche::STL::string name = "Tool";
        Tyche::Input::Key hotkey = Tyche::Input::E;

        //Note: the editor itself deletes this instance.
        Tool* instance = nullptr;
    };
}
