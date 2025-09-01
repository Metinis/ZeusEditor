#version 410 core

layout(std140) uniform View {
  mat4 mat_vp;
};

layout (location = 0) in vec3 a_pos;
layout (location = 3) in vec4 a_color;

out vec4 v_color;

void main() {
  gl_Position = mat_vp * vec4(a_pos, 1.0);
  v_color = a_color;
}
