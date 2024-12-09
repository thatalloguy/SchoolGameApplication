//
// Created by allos on 12/5/2024.
//
#pragma once

#include "Tool.h"
#include "renderer/TileRenderer.h"

namespace MapEntities {
    class Cursor;
}

namespace Tools {


    class PaintTool: public Tools::Tool {

    public:
        void initialize(MapEditor::App& editor_instance) override;
        void update() override;
        void destroy() override;

    private:
        MapEditor::App* _editor = nullptr;
        Tyche::Tile* _cursor = nullptr;
        Tyche::Window* _window = nullptr;
        Tyche::Camera* _camera = nullptr;

        Vector2 last_snapped_position{0, 0};
    };

}
