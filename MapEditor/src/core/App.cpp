//
// Created by allos on 12/5/2024.
//

#include "App.h"

#include <filesystem>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../tools/EraseTool.h"
#include "../tools/PaintTool.h"
#include "core/Mouse.h"
#include "core/IO.h"
#include "../tools/CollisionTool.h"
#include "../tools/EntityTool.h"


#ifdef _WIN32
#include <windows.h>
#endif


MapEditor::App::~App() {
    deinitializeImGui();
    destroyAllTools();
    for (auto name : _room_names) {
        delete name;
    }

    for (auto tile : _current_room.tiles) {
        delete tile;
    }

    for (auto entity: _current_room.entities) {
        delete entity;
    }

    Tyche::Input::cleanUp();
}

void MapEditor::App::init() {
    initializeImGui();

    //Set the window as target to capture input from/
    Tyche::Mouse::setTargetWindow(_window.getRawWindowPtr());
    Tyche::Input::setTargetWindow(_window);


    // Reserve some memory to avoid having to resize alot.
    _tools.reserve(6);
    _current_room.tiles.reserve(20);


    // Register all of the default tools, all allocated memory gets cleaned up in the Deconstructor
    Tools::ToolInfo* paintTool = new Tools::ToolInfo{
        "Paint",
        Tyche::Input::Key::G,
        new Tools::PaintTool,
    };

    Tools::ToolInfo* eraseTool = new Tools::ToolInfo{
        "Erase",
        Tyche::Input::Key::E,
        new Tools::EraseTool,
    };

    Tools::ToolInfo* collisionTool = new Tools::ToolInfo{
            "Collision",
            Tyche::Input::Key::C,
            new Tools::CollisionTool,
    };

    Tools::ToolInfo* entityTool = new Tools::ToolInfo{
        "Entity",
        Tyche::Input::Key::B,
        new Tools::EntityTool,
    };

    registerNewTool(paintTool);
    registerNewTool(eraseTool);
    registerNewTool(collisionTool);
    registerNewTool(entityTool);


    // Painting is the default.
    setCurrentTool(paintTool);

    //Runs initialize for each registered tool.
    initializeTools();

    _tile_renderer.initialize({});
    _entity_renderer.initialize({});



    //Setup inputs
    Tyche::Input::addAction("camera-left");
    Tyche::Input::addAction("camera-right");
    Tyche::Input::addAction("camera-up");
    Tyche::Input::addAction("camera-down");

    Tyche::Input::addKey("camera-left", Tyche::Input::A);
    Tyche::Input::addKey("camera-right", Tyche::Input::D);
    Tyche::Input::addKey("camera-up", Tyche::Input::W);
    Tyche::Input::addKey("camera-down", Tyche::Input::S);


    //Load all rooms
    for (const auto & entry : std::filesystem::directory_iterator("../../../Rooms/")) {
        //may god forgive me for this sin of c++ code
        _room_names.push_back(new string{entry.path().filename().string().data()});
    }
}

void MapEditor::App::run() {


    auto currentTime = std::chrono::high_resolution_clock ::now();

    while (!_window.shouldWindowClose()) {

        // First calculate the frametime (used for updating the camera position)
        auto newTime = std::chrono::high_resolution_clock::now();
        double frameTime = (double) std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count() / 100000;
        currentTime = newTime;

        // If we dont cap it, then the physics engine will explode since instead of correcting the velocities we are enlarging them.
        if (frameTime >= 0.166)
            frameTime = 0.166;

        // Update the window and check for inputs.
        _window.update();
        if (!ImGui::GetIO().WantCaptureKeyboard) {
            checkForToolHotkeys();
            updateCamera(frameTime);
        }


        // Update the canera
        _camera.setViewportSize(_window.getWindowSize());
        _camera.update();
        // Render all of the tiles first
        _tile_renderer.renderTiles(_camera);
        _entity_renderer.renderEntities(_camera);
        // Render the cursor seperate so its always in front of the tiles.
        _entity_renderer.renderEntity(_camera, &_cursor);

        for (auto collider: _current_room.colliders) {
            _debug_renderer.renderBox(collider);
        }

        // render all of our entities as a debug box.
        for (auto entity: _current_room.entities) {
            Vector4 box = {entity->position[0] - 12.5f, entity->position[1] - 12.5f,
                entity->position[0] + 12.5f, entity->position[1] + 12.5f};
            _debug_renderer.renderBox(box);
        }

        _debug_renderer.render(_camera);
        _debug_renderer.clearQueue();


        updateCurrentTool();

        // Render all the debug GUI.
        newImGuiFrame();

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({_window.getWindowSize().getX(), _window.getWindowSize().getY()});
        ImGui::Begin(" ", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginMenuBar()) {

            if (ImGui::BeginMenu("File")) {

                ImGui::MenuItem("New Room");

                if (ImGui::MenuItem("Save Room")) {
                    saveRoomToDisk(_room_names[_current_room_index]->c_str());
                }

                if (ImGui::MenuItem("Load Room")) {
                    loadRoomFromDisk(_room_names[_current_room_index]->c_str());
                    _display_popup = true;
                }

                if (ImGui::MenuItem("Reload Room List")) {
                    //Reload the internal room list :D
                    for (auto name : _room_names) {
                        delete name;
                    }

                    for (const auto & entry : std::filesystem::directory_iterator("../../../Rooms/")) {
                        //may god again forgive me for this sin of c++ code
                        _room_names.push_back(new string{entry.path().filename().string().data()});
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

            // DEPRECATED FEATURE
/*
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
*/



            ImGui::Text("|| Current Room:");
            ImGui::SetNextItemWidth(_window.getWindowSize().getX() * 0.1f);

            //Render a dropdown menu to allow room selection.
            if (ImGui::BeginCombo("Rooms", _room_names[_current_room_index]->c_str())) {
                for (int i=0; i< _room_names.length(); i++) {
                    auto name = _room_names[i];
                    if (ImGui::Selectable(name->c_str()))
                        _current_room_index = i;
                }
                ImGui::EndCombo();
            }



            ImGui::EndMenuBar();
        }

        ImGui::End();

        _current_tool->instance->displayDebugWindow();

        renderImGuiFrame();
    }
}


Tyche::Entity& MapEditor::App::getCursor() {
    return _cursor;
}

Tyche::Window& MapEditor::App::getWindow() {
    return _window;
}

Tyche::Camera& MapEditor::App::getCamera() {
    return _camera;
}

void MapEditor::App::placeTile(Vector2 pos, Tyche::Tile* tile) {
    //place a tile and make the tile renderer refresh.
    _current_room.tiles.push_back(tile);
    _tile_renderer.addTile(*tile);

    _tile_renderer.prepareRendering();

}

void MapEditor::App::removeTile(Vector2 pos) {

     for (int i=0; i<_current_room.tiles.length(); i++) {
         auto tile = _current_room.tiles[i];

         if (tile->position == pos) {
             _current_room.tiles.remove(i);
             _tile_renderer.removeTile(i);
             delete tile;

             // we return to avoid deleting multiple tiles.
             return;
         }
     }

}

void MapEditor::App::addCollider(Vector4& collider) {
    _current_room.colliders.push_back(collider);
}

void MapEditor::App::deleteCollider(const Vector2& begin_position) {
    int i = 0;
    for (auto collider : _current_room.colliders) {
        if (collider[0] == begin_position[0] && collider[1] == begin_position[1]) {
            _current_room.colliders.remove(i);
            return;
        }
        i++;
    }
}

MapEditor::EntityBlueprint* MapEditor::App::addEntity(const Vector2& pos) {
    _current_room.entities.push_back(new EntityBlueprint{.position=pos});

    return _current_room.entities.back();
}

MapEditor::EntityBlueprint* MapEditor::App::getEntity(const Vector2& pos) {

    for (auto entity: _current_room.entities) {
        if (entity->position == pos) {
            return entity;
        }
    }

    return nullptr;
}

void MapEditor::App::deleteEntity(const Vector2& pos) {

    for (int i=0; i<_current_room.entities.length(); i++) {

        auto entity = _current_room.entities[i];

        if (entity->position == pos) {
            delete entity;
            _current_room.entities.remove(i);
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
            _current_tool->instance->onSwitch();
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

void MapEditor::App::displayLoadMenu() {

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

void MapEditor::App::saveRoomToDisk(const char* path) {


    string full_path = "../../../Rooms/";
    full_path = full_path + path;

    FILE* file = fopen(full_path.c_str(), "w");

    Tyche::IO::saveVectorToFile<Tyche::Tile*, Tyche::Tile>(full_path.c_str(), _current_room.tiles, file, false);
    Tyche::IO::saveVectorToFile<EntityBlueprint*, EntityBlueprint>(full_path.c_str(), _current_room.entities, file, false);
    Tyche::IO::saveVectorToFile<Vector4>(full_path.c_str(), _current_room.colliders, file, false);

    spdlog::info("Saved Room to disk: {}", full_path.c_str());

    fclose(file);
}

void MapEditor::App::loadRoomFromDisk(const char* path) {
    for (auto tile : _current_room.tiles) {
        delete tile;
    }

    for (auto entity: _current_room.entities) {
        delete entity;
    }

    _current_room.tiles.clear();
    _current_room.entities.clear();
    _current_room.colliders.clear();

    // correct the path.
    string full_path = "../../../Rooms/";
    full_path = full_path + path;

    FILE* file = fopen(full_path.c_str(), "r");

    Tyche::IO::loadVectorFromFile<Tyche::Tile*, Tyche::Tile>(full_path.c_str(), _current_room.tiles, file, false);
    Tyche::IO::loadVectorFromFile<EntityBlueprint*, EntityBlueprint>(full_path.c_str(), _current_room.entities, file, false);
    Tyche::IO::loadVectorFromFile<Vector4>(full_path.c_str(), _current_room.colliders, file, false);

    for (auto tile : _current_room.tiles) {
        _tile_renderer.addTile(*tile);
    }

    _tile_renderer.prepareRendering();

    spdlog::info("Loaded Room from disk: {}", full_path.c_str());

    fclose(file);
}

Tyche::EntityRenderer &MapEditor::App::getEntityRenderer() {
    return _entity_renderer;
}

Tyche::DebugRenderer& MapEditor::App::getDebugRenderer() {
    return _debug_renderer;
}
