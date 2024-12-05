//
// Created by allos on 11/19/2024.
//
#pragma once

#include "math/vecs.h"
#include "stl/vector.h"
#include "Shader.h"
#include "core/typedefs.h"
#include "math/mat4.h"
#include "Camera.h"
#include "stl/string.h"
#include "Texture.h"

using namespace Tyche::Math;
using namespace Tyche::STL;

namespace Tyche {



    // A Tile is a textured Square that
    struct Tile {
        // Position where the texture is rendered
        Vector2 position{500, 500};
        // The scale of tile (not really used)
        Vector2 scale{100, 100};

        // The final transformation matrix (should include  the scale and position of the tile)
        Matrix4 transform;

        // Type determines the texture the tile will have
        Vector2 texture_pos = {0, 0};
    };


    // A struct to specify how the TileRenderer should be structured
    struct TileRendererCreationInfo {
        int tile_size = 16; // a tile will be 16x16
        int atlas_size = 64; // the entire atlas should be 64x64
        const STL::string texture_path = "../../../Resources/Textures/texture_atlas.png";
    };

    // Tile renderer renderers tiles (in the correct order)
    class TileRenderer {
    public:

        //Default constructor and deconstructor
        TileRenderer() = default;
        ~TileRenderer();

        // initializes the tile mesh and shader.
        void initialize(const TileRendererCreationInfo& creation_info);

        // Adds the tile to an internal list (with the correct layer)
        void addTile(Tile& tile);

        //Clears all tiles from every layer
        void clearTiles();

        //Renders all layers and tiles to the screen.
        void renderTiles(const Camera& camera);

        int getTileSize();
        void setTileSize(int new_tile_size);

        int getAtlasSize();
        void setAtlasSize(int new_atlas_size);

    private:
        vector<Tile*> _tiles;
        GPUMesh _mesh{0, 0};
        Shader _tile_shader;
        // x is the tile_size and y is the atlas size
        Vector2 _grid{16, 64};
        Texture _texture_atlas{};

        void loadTextureAtlas(const string& path);
    };

}
