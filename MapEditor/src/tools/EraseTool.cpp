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
    _cursor->position = mouse_pos;
}

void Tools::EraseTool::onSwitch() {

}

void Tools::EraseTool::destroy() {
}
