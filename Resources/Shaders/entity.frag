#version 420 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D texture_atlas;

void main() {
    vec3 albedo = texture(texture_atlas, TexCoords).rgb;
    fragColor = vec4(albedo, 1);
}