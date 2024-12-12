//
// Created by allos on 12/11/2024.
//
#pragma once


#include "Tool.h"
#include "renderer/TileRenderer.h"
#include "../entities/DebugBox.h"
#include "renderer/DebugRenderer.h"
#include "renderer/EntityRenderer.h"


namespace Tools {

    class CollisionTool: public Tools::Tool {

    public:
        void initialize(MapEditor::App& editor_instance) override;
        void update() override;
        void onSwitch() override;
        void destroy() override;

    private:
        MapEditor::App* _editor = nullptr;
        Tyche::Tile* _cursor = nullptr;
        Tyche::DebugRenderer* _debug_renderer = nullptr;

        Tyche::Camera* _camera = nullptr;

        bool is_holding = false;
        bool has_builded_last_frame = false;

        Vector4 AABB{0, 0, 0, 0};
        Vector2 last_snapped_position{0, 0};
    };

}