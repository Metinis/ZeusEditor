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

layout(std140) uniform Material {
  float specularStrength;
};

in vec4 v_color;
in vec2 v_uv;

in vec3 v_frag_pos;
in vec3 v_normal;

out vec4 fragColor;

void main() {
  //calculate light values
  vec3 norm = normalize(v_normal);
  vec3 lightDir = normalize(lightPos - v_frag_pos);

  //calculate specular
  vec3 viewDir = normalize(cameraPos - v_frag_pos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0); //todo replace with light color

  //calculate diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);  //todo replace with light color

  //calculate and submit result
  vec3 result = (diffuse + ambientColor + specular) * texture(tex, v_uv).rgb;
  fragColor = vec4(result, 1.0);
}
