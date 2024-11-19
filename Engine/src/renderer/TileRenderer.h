//
// Created by allos on 11/19/2024.
//
#pragma once

#include "math/vecs.h"
#include "stl/vector.h"
#include "Shader.h"
#include "core/typedefs.h"
#include "math/mat4.h"

using namespace Tyche::Math;
using namespace Tyche::STL;

namespace Tyche {

    //just a very simple struct to represent a GPU mesh.
    struct TileMesh {
        //VAO = vertex array buffer
        unsigned int VAO = 0;
        //VBO = vertex buffer object
        unsigned int VBO = 0;
    };

    // A Tile is a textured Square that
    struct Tile {
        // Position where the texture is rendered
        Vector2 position{0, 0};
        // The scale of tile (not really used)
        Vector2 scale{1, 1};

        // The final transformation matrix (should include  the scale and position of the tile)
        Matrix4 transform;

        // Type determines the texture the tile will have
        int type = 0;
        //Layer determines in what order the tiles are drawn.
        int layer = 0;
    };

    // Tile renderer renderers tiles (in the correct order)
    class TileRenderer {
    public:

        //Default constructor and deconstructor
        TileRenderer() = default;
        ~TileRenderer();

        // initializes the tile mesh and shader.
        void initialize();

        // Adds the tile to an internal list (with the correct layer)
        void addTile(Tile& tile);

        //Clears all tiles from every layer
        void clearTiles();

        //Renders all layers and tiles to the screen.
        void renderTiles();

    private:
        vector<Tile> _tiles;
        TileMesh _mesh{0, 0};
        Shader _tile_shader;
    };

}
