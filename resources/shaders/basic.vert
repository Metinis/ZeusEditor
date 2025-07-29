#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec4 aColor;

out vec4 vColor;  // output to fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;  // pass per-vertex color to fragment shader
}