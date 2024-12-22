#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 3) in mat4 aMatrix;
layout (location = 7) in vec2 aTexSlot;

out vec2 TexCoords;

//uniform mat4 model;
uniform mat4 projection;

uniform vec2 grid;

void main()
{
    vec2 tileUv = (aTexCoords / (grid.y / grid.x));
    tileUv = tileUv += (aTexSlot / (grid.y / grid.x));
    TexCoords = tileUv;

    gl_Position = projection * aMatrix * vec4(aPos.x , aPos.y, 0.0f, 1.0f);
}