#version 410 core

layout(std140) uniform View {
  mat4 mat_vp;
};

layout(std140) uniform Instances {
  mat4 mat_ms[100];
};


layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec4 a_color;

out vec4 v_color;
out vec2 v_uv;

void main() {
  mat4 mat_m = mat_ms[gl_InstanceID];
  vec4 world_pos = mat_m * vec4(a_pos, 1.0);
  v_color = a_color;
  gl_Position = mat_vp * world_pos;
  v_uv = a_texcoord;
}
