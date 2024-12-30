//
// Created by allos on 12/11/2024.
//
#pragma once


#include "Tool.h"
#include "../core/App.h"
#include "renderer/TileRenderer.h"
#include "renderer/DebugRenderer.h"
#include "renderer/EntityRenderer.h"


namespace Tools {

    // A tool for creating, removing and editing entities.
    class EntityTool: public Tools::Tool {

    public:
        void initialize(MapEditor::App& editor_instance) override;
        void update() override;
        void onSwitch() override;
        void destroy() override;
        void displayDebugWindow() override;

    private:
        MapEditor::App* _editor = nullptr;
        Tyche::Entity* _cursor = nullptr;
        Tyche::DebugRenderer* _debug_renderer = nullptr;

        Tyche::Camera* _camera = nullptr;
        MapEditor::EntityBlueprint* selected_entity = nullptr;

        bool has_builded_last_frame = false;

        Vector4 AABB{0, 0, 0, 0};
    };

}