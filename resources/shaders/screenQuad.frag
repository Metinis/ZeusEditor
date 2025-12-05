#version 410 core
out vec4 fragColor;

in vec2 v_TexCoords;

uniform sampler2D screenTexture;

void main()
{
    fragColor = texture(screenTexture, v_TexCoords);
}