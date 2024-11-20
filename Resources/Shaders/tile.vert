#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


uniform mat4 model;
uniform mat4 projection;

uniform vec2 grid;
uniform vec2 texture_slot;

void main()
{
    vec2 tileUv = (aTexCoords / (grid.y / grid.x));
    tileUv = tileUv += (texture_slot / (grid.y / grid.x));
    TexCoords = tileUv;

    gl_Position = projection * model * vec4(aPos.x , aPos.y, 0.0f, 1.0f);
}