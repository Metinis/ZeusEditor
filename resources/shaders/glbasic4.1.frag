#version 410 core

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;


layout(std140) uniform Globals {
  vec3 u_LightDir;
  vec3 u_CameraPos;
  //float u_Time;
  vec3 u_AmbientColor;
};

layout(std140) uniform Material {
  vec2 u_SpecularAndShininess;
};

in vec4 v_Color;
in vec2 v_UV;
in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 fragColor;

void main() {
  //Normalize vectors
  vec3 normal = normalize(v_Normal);
  vec3 lightDir = normalize(-u_LightDir);
  vec3 viewDir  = normalize(u_CameraPos - v_FragPos);

  //Sample textures
  vec3 albedo = texture(u_DiffuseMap, v_UV).rgb;
  vec3 specMap = texture(u_SpecularMap, v_UV).rgb;

  //Ambient component
  vec3 ambient = u_AmbientColor * albedo;

  //Diffuse component
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * albedo;

  //Specular component
  vec3 reflectDir = reflect(-lightDir, normal);
  float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), u_SpecularAndShininess.y);
  vec3 specular = u_SpecularAndShininess.x * specFactor * specMap;

  //Final color
  vec3 finalColor = ambient + diffuse + specular;
  fragColor = vec4(finalColor, 1.0);
}
