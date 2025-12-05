#version 410 core
layout (location = 0) in vec2 a_Position;
layout (location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
}