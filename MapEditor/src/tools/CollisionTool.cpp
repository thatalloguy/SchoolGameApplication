//
// Created by allos on 12/11/2024.
//

#include "CollisionTool.h"

#include "../core/App.h"
#include "core/Mouse.h"

void Tools::CollisionTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;
    _entity_renderer = &_editor->getEntityRenderer();

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();

    _debug_box.initialize();
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
            _debug_box.setBeginPos(mouse_pos);
        }

        _debug_box.setEndPos(snapped_pos);
        is_holding = true;
    } else {
        is_holding = false;
    }
    _debug_box.update();

    _entity_renderer->renderEntity(*_camera, &_debug_box);
}

void Tools::CollisionTool::onSwitch() {

}

void Tools::CollisionTool::destroy() {

}
