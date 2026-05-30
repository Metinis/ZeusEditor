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
    vec3 envColor = texture(cubeTextures[pc.skyboxIdx], inUV).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    outFragColor = vec4(envColor, 1.0);
}
