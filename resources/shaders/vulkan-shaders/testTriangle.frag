#version 450

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inFragPos;

layout (location = 0) out vec4 outFragColor;

layout(set = 0, binding = 0) uniform SceneData {
    mat4 viewproj;
    vec4 ambientColor;
    vec4 sunlightDirection; // w for sun power
    vec4 sunlightColor;
} SceneDataBuffer;

void main()
{
    vec3 lightDir = normalize(vec3(SceneDataBuffer.sunlightDirection) - inFragPos);
    vec3 norm = normalize(inNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 objectColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * objectColor;
    vec3 ambient = SceneDataBuffer.ambientColor.xyz * SceneDataBuffer.sunlightColor.xyz;

    vec3 color = (ambient + diffuse) * objectColor;
    outFragColor = vec4(color, 1.0);
}
