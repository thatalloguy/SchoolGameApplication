//
// Created by allos on 11/19/2024.
//

#include "TileRenderer.h"
#include <glad/glad.h>


#include <string>
#include <fstream>
#include <spdlog/spdlog.h>

float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
};

std::string loadTxtFromFile(const char* path){
    std::ifstream in(path, std::ios::binary);
    if (in)
    {
        //Read the contents of the file
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());

        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.length());

        in.close();
        // Give
        return contents;
    }
    spdlog::error("Couldn't load: ", path);
    throw(errno);
}



Tyche::TileRenderer::~TileRenderer() {

    glDeleteVertexArrays(1, &_mesh.VAO);

}

void Tyche::TileRenderer::initialize() {

    //Create a tile mesh and upload it to the gpu.
    glGenVertexArrays(1, &_mesh.VAO);
    glGenBuffers(1, &_mesh.VBO);

    glBindVertexArray(_mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    auto vertSRC = loadTxtFromFile("../../../Resources/Shaders/tile.vert");
    auto fragSRC = loadTxtFromFile("../../../Resources/Shaders/tile.frag");

    ShaderCreateInfo tile_shader_info {
        .vertexSRC    = vertSRC.c_str(),
        .fragmentSRC  = fragSRC.c_str()
    };

    _tile_shader.init(tile_shader_info);


}

void Tyche::TileRenderer::addTile(Tyche::Tile& tile) {
    _tiles.push_back(tile);
}

void Tyche::TileRenderer::clearTiles() {
    _tiles.clear();
}

void Tyche::TileRenderer::renderTiles() {
    glViewport(0, 0, 1280, 720);

    _tile_shader.bind();

    Matrix4 camera;
    camera.ortho(0.0f, 1280, 720, 0.0f, -1.0f, 1.0f);

    _tile_shader.setMatrix4("projection", camera.value_ptr());

    for (auto tile: _tiles) {
        glBindVertexArray(_mesh.VAO);
        tile.transform.translate({600, 600.0f});
        tile.transform.scale({200, 200});

        _tile_shader.setMatrix4("model", tile.transform.value_ptr());

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
