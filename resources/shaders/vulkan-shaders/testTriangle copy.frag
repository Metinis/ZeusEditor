#version 450
#extension GL_GOOGLE_include_directive : require
#include "bindless.glsl"

layout (location = 0) out vec4 outFragColor;

layout (location = 0) in vec3 inUV;

layout( push_constant ) uniform PushConstants
{
    uint skyboxIdx;
} pc;

void main()
{
    //10 is placeholder
    outFragColor = texture(cubeTextures[pc.skyboxIdx], inUV);
}
