#version 450
#extension GL_GOOGLE_include_directive : require
#include "bindless.glsl"

layout (location = 0) out vec4 outFragColor;

layout (location = 0) in vec3 inUV;

void main()
{
    //10 is placeholder
    outFragColor = texture(cubeTextures[1], inUV);
}
