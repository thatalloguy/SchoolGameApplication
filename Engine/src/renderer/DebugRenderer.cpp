//
// Created by allos on 12/12/2024.
//

#include "DebugRenderer.h"

//Global constants
namespace {

    GPUMesh _quad_mesh;

}

Tyche::LineDrawCommand::LineDrawCommand(const Vector2& begin, const Vector2& end) {

    float line_verts[] = {
        begin[0], begin[1],  0.0f, 1.0f,
        end[0], end[1],  0.0f, 0.0f
    };

    //Setup a reusable quad mesh;
    glGenVertexArrays(1, &_line_mesh.VAO);
    glGenBuffers(1, &_line_mesh.VBO);

    glBindVertexArray(_line_mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _line_mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(line_verts), &line_verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Tyche::LineDrawCommand::render(Shader& shader, Camera& camera) {
    shader.bind();

    Matrix4 transform{};

    shader.setMatrix4("projection", camera.getMatrix().value_ptr());
    shader.setMatrix4("model", transform.value_ptr());

    glBindVertexArray(_line_mesh.VAO);
    glDrawArrays(GL_LINES, 0, 2);

    glDeleteVertexArrays(1, &_line_mesh.VAO);
}

Tyche::PointDrawCommand::PointDrawCommand(const Vector2& position) {
    _transform.translate(position);
    _transform.scale({10, 10});
}

void Tyche::PointDrawCommand::render(Shader& shader, Camera& camera) {
    shader.bind();

    shader.setMatrix4("projection", camera.getMatrix().value_ptr());
    shader.setMatrix4("model", _transform.value_ptr());

    glBindVertexArray(_quad_mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Tyche::DebugRenderer::DebugRenderer() {


    //Create the debug shader
    auto vertSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/debug.vert");
    auto fragSRC = Utils::loadTxtFromFile("../../../Resources/Shaders/debug.frag");

    ShaderCreateInfo entity_shader_info {
        .vertexSRC    = vertSRC.c_str(),
        .fragmentSRC  = fragSRC.c_str()
    };

    _debug_shader.init(entity_shader_info);


    //Setup a reusable quad mesh;
    glGenVertexArrays(1, &_quad_mesh.VAO);
    glGenBuffers(1, &_quad_mesh.VBO);

    glBindVertexArray(_quad_mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _quad_mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Utils::quadVertices), &Utils::quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    _queue.reserve(10);
}

Tyche::DebugRenderer::~DebugRenderer() {
    glDeleteVertexArrays(1, &_quad_mesh.VAO);
}

void Tyche::DebugRenderer::renderLine(const Vector2& begin, const Vector2& end) {
    _queue.push_back(new LineDrawCommand(begin, end));
}

void Tyche::DebugRenderer::renderPoint(const Vector2& position) {

    _queue.push_back(new PointDrawCommand(position));
}

void Tyche::DebugRenderer::renderBox(const Vector4& AABB) {

    _queue.push_back(new PointDrawCommand({AABB[0], AABB[1]}));
    _queue.push_back(new PointDrawCommand({AABB[2], AABB[1]}));
    _queue.push_back(new PointDrawCommand({AABB[0], AABB[3]}));
    _queue.push_back(new PointDrawCommand({AABB[2], AABB[3]}));

    _queue.push_back(new LineDrawCommand({AABB[0], AABB[1]}, {AABB[2], AABB[1]}));
    _queue.push_back(new LineDrawCommand({AABB[0], AABB[1]}, {AABB[0], AABB[3]}));

    _queue.push_back(new LineDrawCommand({AABB[2], AABB[3]}, {AABB[2], AABB[1]}));
    _queue.push_back(new LineDrawCommand({AABB[2], AABB[3]}, {AABB[0], AABB[3]}));

}

void Tyche::DebugRenderer::render(Camera& camera) {
    for (auto command: _queue) {
        command->render(_debug_shader, camera);

        delete command;
    }

    _queue.clear();
}
