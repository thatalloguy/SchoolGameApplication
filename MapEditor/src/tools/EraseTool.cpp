//
// Created by allos on 12/5/2024.
//

#include "EraseTool.h"

#include "../core/App.h"
#include "core/Mouse.h"

void Tools::EraseTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;

    _cursor = &editor_instance.getCursor();
}

void Tools::EraseTool::update() {
    _cursor->texture_pos = {3, 3};
    _cursor->scale = {50, 50};

    auto mouse_pos = Tyche::Mouse::getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = snapped_pos;

    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) && last_snapped_position != snapped_pos) {
        last_snapped_position = snapped_pos;

        _editor->removeTile(snapped_pos);
    }
}

void Tools::EraseTool::onSwitch() {

}

void Tools::EraseTool::destroy() {
}
