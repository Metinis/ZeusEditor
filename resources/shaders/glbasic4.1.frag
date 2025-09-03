#version 410 core

uniform sampler2D tex;

in vec4 v_color;
in vec2 v_uv;

out vec4 fragColor;

void main() {
  fragColor = texture(tex, v_uv);
}
