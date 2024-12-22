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
        _cursor->setSprite({1, 3});
    } else {
        _cursor->setSprite({0, 3});
    }
    _cursor->setScale({32, 32});

    //Get the mouse position and then snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->setPosition(mouse_pos);
    AABB[0] = snapped_pos[0] - 12.5f;
    AABB[1] = snapped_pos[1] - 12.5f;

    AABB[2] = snapped_pos[0] + 12.5f;
    AABB[3] = snapped_pos[1] + 12.5f;


    _debug_renderer->renderBox(AABB);


    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::RIGHT)) {
        if (!Tyche::Input::isKeyPressed(Tyche::Input::LSHIFT)  && !has_builded_last_frame) {
            selected_entity = _editor->addEntity(snapped_pos);
            has_builded_last_frame = true;
        } else {

            if (Tyche::Input::isKeyPressed(Tyche::Input::LCONTROL)) {
                _editor->deleteEntity(snapped_pos);
                selected_entity = nullptr;
            }
            else
                selected_entity = _editor->getEntity(snapped_pos);

        }

    } else {
        has_builded_last_frame = false;
    }
}

void Tools::EntityTool::onSwitch() {
    selected_entity = nullptr;
}

void Tools::EntityTool::destroy() {

}

void Tools::EntityTool::displayDebugWindow() {

    if (selected_entity != nullptr) {
        ImGui::Begin("Entity Info");

        ImGui::DragInt("Type", &selected_entity->type);

        ImGui::InputText("Tags", selected_entity->tags, 50);

        ImGui::End();
    }


}
