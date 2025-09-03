#version 410 core

uniform sampler2D tex;

layout(std140) uniform Globals {
  vec3 lightPos;
  float _pad1;
  vec3 cameraPos;
  float _pad2;
  float time;
  vec3 ambientColor;
};

in vec4 v_color;
in vec2 v_uv;

in vec3 v_frag_pos;
in vec3 v_normal;

out vec4 fragColor;

void main() {
  vec3 norm = normalize(v_normal);
  vec3 lightDir = normalize(lightPos - v_frag_pos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
  vec3 result = (diffuse + ambientColor) * texture(tex, v_uv).rgb;
  fragColor = vec4(result, 1.0);
}
