//
// Created by allos on 12/11/2024.
//

#include "EntityTool.h"

#include "imgui.h"
#include "../core/App.h"
#include "core/Mouse.h"

void Tools::EntityTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;
    _debug_renderer = &_editor->getDebugRenderer();

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();
}

void Tools::EntityTool::update() {
    if (!Tyche::Input::isKeyPressed(Tyche::Input::LSHIFT)) {
        _cursor->texture_pos = {1, 3};
    } else {
        _cursor->texture_pos = {0, 3};
    }
    _cursor->scale = {32, 32};

    //Get the mouse position and then snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = mouse_pos;
    AABB[0] = snapped_pos[0] - 12.5f;
    AABB[1] = snapped_pos[1] - 12.5f;

    AABB[2] = snapped_pos[0] + 12.5f;
    AABB[3] = snapped_pos[1] + 12.5f;

    _debug_renderer->renderBox(AABB);


    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::RIGHT) && !has_builded_last_frame ) {
        has_builded_last_frame = true;
    } else {
        has_builded_last_frame = false;
    }
}

void Tools::EntityTool::onSwitch() {

}

void Tools::EntityTool::destroy() {

}

void Tools::EntityTool::displayDebugWindow() {
    ImGui::Begin("Entity Info");



    ImGui::End();
}
