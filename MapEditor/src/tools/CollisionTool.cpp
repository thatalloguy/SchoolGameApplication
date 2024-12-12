//
// Created by allos on 12/11/2024.
//

#include "CollisionTool.h"

#include "../core/App.h"
#include "core/Mouse.h"

void Tools::CollisionTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;
    _debug_renderer = &_editor->getDebugRenderer();

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();
}

void Tools::CollisionTool::update() {
    _cursor->texture_pos = {2, 3};
    _cursor->scale = {32, 32};

    //Get the mouse position and then snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = mouse_pos;
    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT)) {
        if (!is_holding) {
            AABB[0] = snapped_pos[0];
            AABB[1] = snapped_pos[1];
        }

        AABB[2] = snapped_pos[0];
        AABB[3] = snapped_pos[1];

        is_holding = true;
    } else {
        is_holding = false;
    }

    _debug_renderer->renderBox(AABB);
}

void Tools::CollisionTool::onSwitch() {

}

void Tools::CollisionTool::destroy() {

}
