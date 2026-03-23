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
    float diff = max(dot(inNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    outFragColor = vec4(diffuse,1.0f);
}
