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
    if (!Tyche::Input::isKeyPressed(Tyche::Input::LSHIFT)) {
        _cursor->setSprite({2, 3});
    } else {
        _cursor->setSprite({3, 2});
    }
    _cursor->setScale({32, 32});

    //Get the mouse position and then snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->setPosition(mouse_pos);


    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) ) {

        // If we weren't pressing the mouse button, yet then it means this will be our start pos.
        if (!is_holding) {
            AABB[0] = snapped_pos[0];
            AABB[1] = snapped_pos[1];
        }
        // Always set the end position to the mouse.
        AABB[2] = snapped_pos[0];
        AABB[3] = snapped_pos[1];

        is_holding = true;
        _debug_renderer->renderBox(AABB);
    } else {
        is_holding = false;
    }


    if (Tyche::Input::isKeyPressed(Tyche::Input::Z) && !has_builded_last_frame) {
        //build

        // just check if the sprite is 2 on the x-axis to avoid colling isPressed unneeded.
        if (_cursor->getSprite()[0] == 2) {
            _editor->addCollider(AABB);
        } else {
            _editor->deleteCollider(snapped_pos);
        }

        has_builded_last_frame = true;
    } if (!Tyche::Input::isKeyPressed(Tyche::Input::Z)) {
        has_builded_last_frame = false;
    }
}

void Tools::CollisionTool::onSwitch() {

}

void Tools::CollisionTool::destroy() {

}
