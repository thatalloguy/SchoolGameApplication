//
// Created by allos on 11/20/2024.
//

#include "EntityRenderer.h"
#include <glad/glad.h>


#include <string>
#include <spdlog/spdlog.h>

Tyche::EntityRenderer::~EntityRenderer() {

    glDeleteVertexArrays(1, &_mesh.VAO);

}

void Tyche::EntityRenderer::initialize(const EntityRendererCreationInfo& creation_info) {

    //Create a Entity mesh and upload it to the gpu.
    glGenVertexArrays(1, &_mesh.VAO);
    glGenBuffers(1, &_mesh.VBO);

    glBindVertexArray(_mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::quadVertices), &Utils::quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //setup the entity shader
    auto vertSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/entity.vert");
    auto fragSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/entity.frag");

    ShaderCreateInfo entity_shader_info {
            .vertexSRC    = vertSRC.c_str(),
            .fragmentSRC  = fragSRC.c_str()
    };

    _entity_shader.init(entity_shader_info);

    _grid.setX((float) creation_info.texture_size);
    _grid.setY((float) creation_info.atlas_size);

    //load the texture atlas
    loadTextureAtlas(creation_info.texture_path);
}

void Tyche::EntityRenderer::addEntity(Tyche::Entity* entity) {
    _entities.push_back(entity);
}

void Tyche::EntityRenderer::clearEntities() {
    _entities.clear();
}

void Tyche::EntityRenderer::renderEntities(const Tyche::Camera &camera) {

    //upload our camera projection to the shader
    _entity_shader.bind();
    _entity_shader.setMatrix4("projection", camera.getMatrix().value_ptr());

    //upload our texture + our texture info to the shader
    _texture_atlas.bind();
    _entity_shader.setInt("texture_atlas", 0);
    _entity_shader.setVector2("grid", _grid.value_ptr());

    for (auto entity_ptr: _entities) {

        Entity entity = *entity_ptr;

        //Bind the mesh
        glBindVertexArray(_mesh.VAO);

        // prepare the entity for rendering
        entity.prepareRendering();

        _entity_shader.setMatrix4("model", entity.getTransform().value_ptr());
        _entity_shader.setVector2("texture_slot", entity.getSprite().value_ptr());

        //Draw the mesh
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}


void Tyche::EntityRenderer::renderEntity(const Tyche::Camera &camera, Tyche::Entity *entity_ptr) {

    Tyche::Entity entity = *entity_ptr;

    //upload our camera projection to the shader
    _entity_shader.bind();
    _entity_shader.setMatrix4("projection", camera.getMatrix().value_ptr());

    //upload our texture + our texture info to the shader
    _texture_atlas.bind();
    _entity_shader.setInt("texture_atlas", 0);
    _entity_shader.setVector2("grid", _grid.value_ptr());

        //Bind the mesh
    glBindVertexArray(_mesh.VAO);

    // prepare the entity for rendering
    entity.prepareRendering();

    _entity_shader.setMatrix4("model", entity.getTransform().value_ptr());
    _entity_shader.setVector2("texture_slot", entity.getSprite().value_ptr());

    //Draw the mesh
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Tyche::EntityRenderer::setTextureSize(int new_texture_size) {
    _grid.setX((float) new_texture_size);
}

void Tyche::EntityRenderer::setAtlasSize(int new_atlas_size) {
    _grid.setY((float) new_atlas_size);
}

void Tyche::EntityRenderer::loadTextureAtlas(const STL::string &path) {

    TextureCreationInfo atlas_texture_info {
            .path = path.c_str(),
            .flip_vertically = false,

            .minFilter = FilterType::NEAREST,
            .magFilter = FilterType::NEAREST,
            .color = TextureColor::RGBA,
    };

    _texture_atlas.init(atlas_texture_info);
}

