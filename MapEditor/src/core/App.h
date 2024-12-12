//
// Created by allos on 12/5/2024.
//

#ifndef CAMERA_SPEED
#define CAMERA_SPEED 200
#include "renderer/DebugRenderer.h"
#endif

#pragma once

#include "../tools/Tool.h"
#include "core/Window.h"
#include "renderer/TileRenderer.h"
#include "stl/vector.h"
#include "renderer/EntityRenderer.h"

namespace MapEditor {

    //Simple struct that holds all info we need about a room
    // NOTE: every ptr in the internal vector gets cleaned up by the manager.
    struct Room {
        vector<Tyche::Tile*> tiles;
        vector<Vector4> colliders;
    };


    // The Editor Master class.
    class App {

    public:
        App() = default;
        ~App();

        void init();
        void run();

        Tyche::Tile& getCursor();
        Tyche::Window& getWindow();
        Tyche::Camera& getCamera();
        Tyche::EntityRenderer& getEntityRenderer();
        Tyche::DebugRenderer& getDebugRenderer();

        //Places a tile and adds it to the current room
        void placeTile(Vector2 pos, Tyche::Tile* tile);
        //Deletes the tile with this position both from memory and internal buffers.
        void removeTile(Vector2 pos);

        void addCollider(Vector4& collider);
        void deleteCollider(const Vector2& begin_position);

    private:
        Tyche::Window _window{{{1280, 720}, "TombCrawler - Map Editor"}};
        Tyche::TileRenderer _tile_renderer{};
        Tyche::EntityRenderer _entity_renderer{};
        Tyche::DebugRenderer _debug_renderer{};
        Tyche::Camera _camera{{1280, 720}};

        vector<Tools::ToolInfo*> _tools;
        Tools::ToolInfo* _current_tool = nullptr;


        Room _current_room{};

        //default Debug Entities;
        Tyche::Tile _cursor{.scale = {30, 30}, .texture_pos = {3, 3}};

        //Tool functions
        void registerNewTool(Tools::ToolInfo* tool_info);
        void setCurrentTool(Tools::ToolInfo* tool_info);
        void checkForToolHotkeys();
        void initializeTools();
        void updateCurrentTool();
        void destroyAllTools();

        //Imgui funcs
        void initializeImGui();
        void deinitializeImGui();
        void newImGuiFrame();
        void renderImGuiFrame();

        //misc funcs
        void displayLoadMenu();
        void updateCamera(float frameTime);

        //Serialization
        void saveRoomToDisk(const char* path);
        void loadRoomFromDisk(const char* path);
    };

}
