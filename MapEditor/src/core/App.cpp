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
#include "core/IO.h"


#ifdef _WIN32
#include <windows.h>
#endif

MapEditor::App::~App() {
    deinitializeImGui();
    destroyAllTools();

    for (auto tile : _current_room.tiles) {
        delete tile;
    }
}

void MapEditor::App::init() {
    initializeImGui();

    Tyche::Mouse::setTargetWindow(_window.getRawWindowPtr());
    Tyche::Input::setTargetWindow(_window);

    _tools.reserve(6);
    _current_room.tiles.reserve(20);


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

    initializeTools();


    //Setup inputs
    Tyche::Input::addAction("camera-left");
    Tyche::Input::addAction("camera-right");
    Tyche::Input::addAction("camera-up");
    Tyche::Input::addAction("camera-down");

    Tyche::Input::addKey("camera-left", Tyche::Input::A);
    Tyche::Input::addKey("camera-right", Tyche::Input::D);
    Tyche::Input::addKey("camera-up", Tyche::Input::W);
    Tyche::Input::addKey("camera-down", Tyche::Input::S);
}

void MapEditor::App::run() {


    auto currentTime = std::chrono::high_resolution_clock ::now();

    while (!_window.shouldWindowClose()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        double frameTime = (double) std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count() / 100000;
        currentTime = newTime;

        // If we dont cap it, then the physics engine will explode since instead of correcting the velocities we are enlarging them.
        if (frameTime >= 0.166)
            frameTime = 0.166;

        _window.update();
        checkForToolHotkeys();

        _camera.setViewportSize(_window.getWindowSize());
        updateCamera(frameTime);
        _camera.update();

        _tile_renderer.renderTiles(_camera);
        _tile_renderer.renderTile(_camera, _cursor);

        updateCurrentTool();


        newImGuiFrame();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({_window.getWindowSize().getX(), _window.getWindowSize().getY()});
        ImGui::Begin(" ", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginMenuBar()) {

            if (ImGui::BeginMenu("File")) {

                ImGui::MenuItem("New Room");

                if (ImGui::MenuItem("Save Room")) {
                    Tyche::IO::saveVectorToFile<Tyche::Tile*, Tyche::Tile>("../../../test.room", _current_room.tiles);
                }

                if (ImGui::MenuItem("Load Room")) {
                    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>("../../../test.room", _current_room.tiles);

                    for (auto tile : _current_room.tiles) {
                        _tile_renderer.addTile(*tile);
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {

                for (auto tool : _tools) {
                    ImGui::MenuItem(tool->name.c_str());

                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Execute")) {

                if (ImGui::MenuItem("Debug Current Room")) {
                    spdlog::info("Debugging current Room");
#ifdef WIN32
                    system("start ../../Game/Debug/tyche-game.exe -d  \"../../../test.room\"");
#else
                    spdlog::error("Debugging Rooms is only supported on the windows platform");
#endif
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        renderImGuiFrame();
    }
}


Tyche::Tile& MapEditor::App::getCursor() {
    return _cursor;
}

Tyche::Window& MapEditor::App::getWindow() {
    return _window;
}

Tyche::Camera& MapEditor::App::getCamera() {
    return _camera;
}

void MapEditor::App::placeTile(Vector2 pos, Tyche::Tile* tile) {
    _current_room.tiles.push_back(tile);
    _tile_renderer.addTile(*tile);

}

void MapEditor::App::removeTile(Vector2 pos) {

     for (int i=0; i<_current_room.tiles.length(); i++) {
         auto tile = _current_room.tiles[i];

         if (tile->position == pos) {
             _current_room.tiles.remove(i);
             _tile_renderer.removeTile(i);
             delete tile;
             return;
         }
     }

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

void MapEditor::App::updateCamera(float frameTime) {

    auto out_vec = _camera.getPosition();

    if (Tyche::Input::isActionPressed("camera-left")) {
        out_vec += {CAMERA_SPEED * frameTime, 0};
    }


    if (Tyche::Input::isActionPressed("camera-right")) {
        out_vec += {-CAMERA_SPEED  * frameTime, 0};
    }

    if (Tyche::Input::isActionPressed("camera-up")) {
        out_vec += {0, CAMERA_SPEED * frameTime};
    }


    if (Tyche::Input::isActionPressed("camera-down")) {
        out_vec += {0, -CAMERA_SPEED * frameTime};
    }

    _camera.setPosition(out_vec);
}
