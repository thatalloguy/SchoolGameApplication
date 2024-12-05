//
// Created by allos on 12/5/2024.
//

#include "EraseTool.h"

#include "../core/App.h"
#include "core/Mouse.h"

void Tools::EraseTool::initialize(MapEditor::App& editor_instance) {
    _editor = &editor_instance;

    _cursor = &editor_instance.getCursorEntity();
}

void Tools::EraseTool::update() {
    _cursor->updateSprite({3, 0});

    auto mouse_pos = Tyche::Mouse::getPosition();
    _cursor->setPosition(mouse_pos);
}

void Tools::EraseTool::onSwitch() {

}

void Tools::EraseTool::destroy() {
}
