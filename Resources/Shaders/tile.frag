#version 420 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D texture_atlas;

void main() {
    vec4 albedo = texture(texture_atlas, TexCoords).rgba;
    if(albedo.a < 0.1)
        discard;
    fragColor = albedo;
}