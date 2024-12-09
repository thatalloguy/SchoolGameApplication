//
// Created by allos on 12/5/2024.
//

#include "PaintTool.h"
#include "../core/App.h"
#include "core/Mouse.h"


namespace {
    Vector2 tile_texture{0, 0};

    void scrollcallback(GLFWwindow* window, double x, double y) {
        tile_texture += {(float) y / (float) y, 0};

        if (tile_texture[0] > 3) {
            tile_texture.setX(0);
            tile_texture += {0, 1};
        }

        if (tile_texture[0] < 0) {
            tile_texture.setX(0);
            tile_texture += {0, 1};
        }


        if (tile_texture[1] > 3 || tile_texture[1] < 0) {
            tile_texture.setY(0);
            tile_texture.setX(0);
        }

        spdlog::info("TILE {} {}", tile_texture[0], tile_texture[1]);
    }
}

void Tools::PaintTool::initialize(MapEditor::App& editor_instance) {

    _editor = &editor_instance;
    _window = &_editor->getWindow();

    glfwSetScrollCallback((GLFWwindow*) _window->getRawWindowPtr(), scrollcallback);

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();
}

void Tools::PaintTool::update() {

    _cursor->texture_pos = tile_texture;
    _cursor->scale = {25, 25};

    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();

    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->position = snapped_pos;

    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) && last_snapped_position != snapped_pos && mouse_pos[1] > _window->getWindowPosition().getY() + 20) {
        last_snapped_position = snapped_pos;

        _editor->placeTile(snapped_pos, new Tyche::Tile{.position = snapped_pos, .texture_pos = _cursor->texture_pos});
    }
}

void Tools::PaintTool::destroy() {

}
