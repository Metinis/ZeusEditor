#version 410 core

uniform sampler2D tex;

in vec4 v_color;
in vec2 v_uv;

out vec4 out_color;

void main() {
  out_color = texture(tex, v_uv);
}
