#version 410 core

layout(std140) uniform View {
  mat4 u_VP;
};

layout(std140) uniform Instances {
  mat4 u_Model[100];
};
layout(std140) uniform Globals {
  vec3 u_LightPos;
  vec3 u_CameraPos;
//float u_Time;
  vec3 u_AmbientColor;
};
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec3 a_Tangent;
layout(location = 5) in vec3 a_Bitangent;

out vec4 v_Color;
out vec2 v_UV;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_AmbientColor;
out vec3 v_TangentLightPos;
out vec3 v_TangentViewPos;
out vec3 v_TangentFragPos;
out mat3 v_TBN;

void main() {
  mat4 model = u_Model[gl_InstanceID];
  //Pass per-vertex attributes to fragment shader
  v_Color = a_Color;
  v_UV = a_TexCoord;
  v_FragPos = vec3(model * vec4(a_Position, 1.0));

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 T = normalize(normalMatrix * a_Tangent);
  vec3 N = normalize(normalMatrix * a_Normal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);

  mat3 TBN = mat3(T, B, N);
  v_TBN = TBN;
  v_TangentLightPos = TBN * u_LightPos;
  v_TangentViewPos  = TBN * u_CameraPos;
  v_TangentFragPos  = TBN * v_FragPos;

  //Transform vertex position to clip space
  gl_Position = u_VP * model * vec4(a_Position, 1.0);

  // Transform normals to world space
  v_AmbientColor = u_AmbientColor;
  v_Normal = mat3(transpose(inverse(model))) * a_Normal;
}