#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec4 a_color;

layout (location = 0) out vec4 out_color;

void main() {
  gl_Position = vec4(a_pos, 1.0);
  out_color = a_color;
}
