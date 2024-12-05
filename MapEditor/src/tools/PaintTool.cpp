//
// Created by allos on 12/5/2024.
//

#include "PaintTool.h"
#include "../core/App.h"
#include "core/Mouse.h"

void Tools::PaintTool::initialize(MapEditor::App& editor_instance) {

    _editor = &editor_instance;

    _cursor = &editor_instance.getCursor();
}

void Tools::PaintTool::update() {

    _cursor->texture_pos = {0, 0};
    _cursor->scale = {100, 100};

    auto mouse_pos = Tyche::Mouse::getPosition();

    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = snapped_pos;
    spdlog::info("Snapped position: {} {}", snapped_pos[0], snapped_pos[1]);
}

void Tools::PaintTool::destroy() {

}
