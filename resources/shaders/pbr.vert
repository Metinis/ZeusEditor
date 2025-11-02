#version 410 core

layout(std140) uniform View {
  mat4 u_VP;
};

layout(std140) uniform Instances {
  mat4 u_Model[100];
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

  //Transform vertex position to clip space
  gl_Position = u_VP * model * vec4(a_Position, 1.0);

  // Transform normals to world space
  v_Normal = mat3(transpose(inverse(model))) * a_Normal;
}