#version 450 core

layout (set = 0, binding = 0) uniform View {
  mat4 mat_vp;
};

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec4 a_color;

layout (location = 0) out vec4 out_color;

void main() {
  const vec4 world_pos = vec4(a_pos, 1.0);
  out_color = a_color;
  gl_Position = mat_vp * world_pos;
  
}
