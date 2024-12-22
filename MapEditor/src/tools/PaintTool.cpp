//
// Created by allos on 12/5/2024.
//

#include "PaintTool.h"
#include "../core/App.h"
#include "core/Mouse.h"


namespace {
    Vector2 tile_texture{0, 0};

    void scrollcallback(GLFWwindow* window, double x, double y) {


        /* The Tilemap goes from 0 to 3
         * which means 16 textures in total
        */

        // We divide y by itself so that no matter how fast you scroll you will only go up 1.
        tile_texture += {(float) y / (float) y, 0};

        // Clamp the x coord and go to the next row
        if (tile_texture[0] > 3) {
            tile_texture.setX(0);
            tile_texture += {0, 1};
        }

        // Also clamping the X coord but go back up a row
        if (tile_texture[0] < 0) {
            tile_texture.setX(0);
            tile_texture += {0, 1};
        }


        // Reset the y coord if we go outside of the possible range.
        if (tile_texture[1] > 3 || tile_texture[1] < 0) {
            tile_texture.setY(0);
            tile_texture.setX(0);
        }

    }
}

void Tools::PaintTool::initialize(MapEditor::App& editor_instance) {

    _editor = &editor_instance;
    _window = &_editor->getWindow();

    //Set scroll function which is used for selecting different tiles.
    glfwSetScrollCallback((GLFWwindow*) _window->getRawWindowPtr(), scrollcallback);

    _cursor = &editor_instance.getCursor();
    _camera = &editor_instance.getCamera();
}

void Tools::PaintTool::update() {

    //Update the cursor to the current selected tile_texture.
    _cursor->setSprite({2, 2});
    _cursor->setScale({25, 25});

    //Get the mouse position and snap it to the grid.
    auto mouse_pos =  Tyche::Mouse::getPosition() - _camera->getPosition();
    auto snapped_pos = Vector2{floor(mouse_pos[0] / GRID_SIZE), floor(mouse_pos[1] / GRID_SIZE)} * GRID_SIZE;

    _cursor->setPosition(snapped_pos);

    // Only paint if the cursor is in a new grid space and we arent hovering over the menu bar and the left mouse button is down.
    if (Tyche::Mouse::isMouseButtonPressed(Tyche::Mouse::LEFT) && last_snapped_position != snapped_pos && mouse_pos[1] > _window->getWindowPosition().getY() + 20) {
        last_snapped_position = snapped_pos;

        // We can allocate memory here since the Room deletes all tiles in its internal vector
        _editor->placeTile(snapped_pos, new Tyche::Tile{.position = snapped_pos, .texture_pos = tile_texture});
    }
}

void Tools::PaintTool::destroy() {

}
