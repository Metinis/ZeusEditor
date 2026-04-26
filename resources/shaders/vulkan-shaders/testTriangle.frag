#version 450
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_nonuniform_qualifier : require

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inFragPos;
layout (location = 3) in vec2 inUV;
layout (location = 4) flat in uint inAlbedoIdx;

layout (location = 0) out vec4 outFragColor;

layout(set = 0, binding = 0) uniform SceneData {
    mat4 viewproj;
    vec4 ambientColor;
    vec4 sunlightDirection; // w for sun power
    vec4 sunlightColor;
} SceneDataBuffer;

layout(set = 1, binding = 0) uniform sampler2D textures[];

void main()
{
    vec3 lightDir = normalize(vec3(SceneDataBuffer.sunlightDirection) - inFragPos);
    vec3 norm = normalize(inNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 objectColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * objectColor;
    vec3 ambient = SceneDataBuffer.ambientColor.xyz * SceneDataBuffer.sunlightColor.xyz;

    vec3 color = (ambient + diffuse) * texture(textures[inAlbedoIdx], inUV).xyz;
    outFragColor = vec4(color, 1.0);
}
