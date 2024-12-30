//
// Created by allos on 11/20/2024.
//
#pragma once

#include "stl/string.h"
#include "core/Entity.h"
#include "Camera.h"
#include "core/typedefs.h"
#include "Shader.h"
#include "Texture.h"
#include "stl/vector.h"

namespace Tyche {

    //The EntityRenderer is super simuliar to the TileRenderer, but instead it uses entities.

    // Struct to pass along information needed by the creation of the EntityRenderer.
    struct EntityRendererCreationInfo {
        int texture_size = 16; // a tile will be 16x16
        int atlas_size = 64; // the entire atlas should be 64x64
        const STL::string texture_path = "../../../Resources/Textures/entity_atlas.png";
    };

    //The EntityRenderer only RENDERS entities, does not manage them.
    class EntityRenderer {
    public:
        EntityRenderer() = default;
        ~EntityRenderer();

        // Initializes our atlas texture, entity mesh and shader
        void initialize(const EntityRendererCreationInfo& creation_info);

        void addEntity(Entity* entity);
        void clearEntities();
        // Calls the prepareRendering function on every entity and renders them
        void renderEntities(const Camera& camera);
        void renderEntity(const Camera& camera, Entity* entity);


        void setTextureSize(int new_texture_size);
        void setAtlasSize(int new_atlas_size);

        unsigned int getVAO() {return _mesh.VAO; };

    private:
        STL::vector<Entity*> _entities;
        GPUMesh _mesh{0, 0};
        Shader _entity_shader;
        // x is the tile_size and y is the atlas size
        Math::Vector2 _grid{16, 16};
        Texture _texture_atlas{};

        void loadTextureAtlas(const STL::string& path);
    };
}