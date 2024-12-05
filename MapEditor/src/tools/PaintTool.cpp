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

    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) && last_snapped_position != snapped_pos) {
        last_snapped_position = snapped_pos;

        _editor->placeTile(snapped_pos, new Tyche::Tile{.position = snapped_pos, .texture_pos = _cursor->texture_pos});
    }
}

void Tools::PaintTool::destroy() {

}
