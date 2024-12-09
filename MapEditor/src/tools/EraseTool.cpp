//
// Created by allos on 12/5/2024.
//

#include "EraseTool.h"

#include "../core/App.h"
#include "core/Mouse.h"

void Tools::EraseTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;
    _window = &_editor->getWindow();

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();
}

void Tools::EraseTool::update() {
    // Set the cursor to the Erasor sprite.
    _cursor->texture_pos = {3, 3};
    _cursor->scale = {50, 50};

    //Get the mouse position and then snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = mouse_pos;

    // Only erase if the cursor is in a new grid space and we arent hovering over the menu bar and the left mouse button is down.
    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) && last_snapped_position != snapped_pos && mouse_pos[1] > _window->getWindowPosition().getY() + 20) {
        last_snapped_position = snapped_pos;

        _editor->removeTile(snapped_pos);
    }
}

void Tools::EraseTool::onSwitch() {

}

void Tools::EraseTool::destroy() {
}
