//
// Created by allos on 11/19/2024.
//

#include "TileRenderer.h"
#include <glad/glad.h>


#include <string>
#include <fstream>
#include <spdlog/spdlog.h>


Tyche::TileRenderer::~TileRenderer() {

    glDeleteVertexArrays(1, &_mesh.VAO);

}

void Tyche::TileRenderer::initialize(const TileRendererCreationInfo& creation_info) {

    //Create a tile mesh and upload it to the gpu.
    glGenVertexArrays(1, &_mesh.VAO);
    glGenBuffers(1, &_mesh.VBO);

    glBindVertexArray(_mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::quadVertices), &Utils::quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //setup the tile shader
    auto vertSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/tile.vert");
    auto fragSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/tile.frag");

    ShaderCreateInfo tile_shader_info {
        .vertexSRC    = vertSRC.c_str(),
        .fragmentSRC  = fragSRC.c_str()
    };

    _tile_shader.init(tile_shader_info);

    _grid.setX((float) creation_info.tile_size);
    _grid.setY((float) creation_info.atlas_size);

    //load the texture atlas
    loadTextureAtlas(creation_info.texture_path);
}

void Tyche::TileRenderer::addTile(Tyche::Tile& tile) {
    _tiles.push_back(tile);
}

void Tyche::TileRenderer::clearTiles() {
    _tiles.clear();
}

void Tyche::TileRenderer::renderTiles(const Camera& camera) {

    //upload our camera projection to the shader
    _tile_shader.bind();
    _tile_shader.setMatrix4("projection", camera.getMatrix().value_ptr());

    //upload our texture + our texture info to the shader
    _texture_atlas.bind();
    _tile_shader.setInt("texture_atlas", 0);
    _tile_shader.setVector2("grid", _grid.value_ptr());

    for (auto tile: _tiles) {
        //Bind the mesh
        glBindVertexArray(_mesh.VAO);

        // update the transform matrix of the tile
        tile.transform.translate(tile.position);
        tile.transform.scale(tile.scale);

        _tile_shader.setMatrix4("model", tile.transform.value_ptr());
        _tile_shader.setVector2("texture_slot", tile.texture_pos.value_ptr());

        //Draw the mesh
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

int Tyche::TileRenderer::getTileSize() {
    return (int) _grid.getX();
}

void Tyche::TileRenderer::setTileSize(int new_tile_size) {
    _grid.setX((float) new_tile_size);
}

int Tyche::TileRenderer::getAtlasSize() {
    return (int) _grid.getY();
}

void Tyche::TileRenderer::setAtlasSize(int new_atlas_size) {
    _grid.setY((float) new_atlas_size);
}

void Tyche::TileRenderer::loadTextureAtlas(const string &path) {

    TextureCreationInfo atlas_texture_info {
        .path = path.c_str(),
        .flip_vertically = false,

        .minFilter = FilterType::NEAREST,
        .magFilter = FilterType::NEAREST,
        .color = TextureColor::RGBA,
    };

    _texture_atlas.init(atlas_texture_info);
}
