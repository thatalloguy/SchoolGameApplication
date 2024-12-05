//
// Created by allos on 12/5/2024.
//

#include "App.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../tools/EraseTool.h"
#include "../tools/PaintTool.h"
#include "core/Mouse.h"


MapEditor::App::~App() {
    deinitializeImGui();
    destroyAllTools();
}

void MapEditor::App::init() {
    initializeImGui();

    Tyche::Mouse::setTargetWindow(_window.getRawWindowPtr());
    Tyche::Input::setTargetWindow(_window);

    _tools.reserve(6);


    Tools::ToolInfo* paintTool = new Tools::ToolInfo{
        "Paint",
        Tools::ToolType::MouseTool,
        Tyche::Input::Key::G,
        new Tools::PaintTool,
    };

    Tools::ToolInfo* eraseTool = new Tools::ToolInfo{
        "Erase",
        Tools::ToolType::MouseTool,
        Tyche::Input::Key::E,
        new Tools::EraseTool,
    };

    registerNewTool(paintTool);
    registerNewTool(eraseTool);


    setCurrentTool(paintTool);



    _tile_renderer.initialize({});
    _tile_renderer.addTile(_cursor);

    initializeTools();

}

void MapEditor::App::run() {
    while (!_window.shouldWindowClose()) {

        _window.update();
        checkForToolHotkeys();

        _camera.setViewportSize(_window.getWindowSize());
        _camera.update();

        _tile_renderer.renderTiles(_camera);

        newImGuiFrame();

        ImGui::Begin("Hello Window");

        ImGui::Text("Current Tool: %s", _current_tool->name.c_str());

        ImGui::End();

        updateCurrentTool();

        renderImGuiFrame();
    }
}


Tyche::Tile& MapEditor::App::getCursor() {
    return _cursor;
}


void MapEditor::App::registerNewTool(Tools::ToolInfo* tool_info) {
    if (tool_info->instance == nullptr) {
        spdlog::error("Could not register Tool: {} because instance is a nullptr", tool_info->name.c_str());
        return;
    }

    _tools.push_back(tool_info);
}

void MapEditor::App::setCurrentTool(Tools::ToolInfo* tool_info) {
    _current_tool = tool_info;
}

void MapEditor::App::checkForToolHotkeys() {
    for (auto tool : _tools) {
        if (Tyche::Input::isKeyPressed(tool->hotkey)) {
            setCurrentTool(tool);
        }
    }
}

void MapEditor::App::initializeTools() {
    for (auto tool : _tools) {
        tool->instance->initialize(*this);
    }
}

void MapEditor::App::updateCurrentTool() {
    _current_tool->instance->update();
}

void MapEditor::App::destroyAllTools() {
    for (auto tool : _tools) {
        tool->instance->destroy();

        delete tool->instance;
        delete tool;
    }
}



void MapEditor::App::initializeImGui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) _window.getRawWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 420");
}

void MapEditor::App::deinitializeImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MapEditor::App::newImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void MapEditor::App::renderImGuiFrame() {

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
