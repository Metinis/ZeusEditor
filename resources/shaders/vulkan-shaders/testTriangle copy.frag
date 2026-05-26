#version 450
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_nonuniform_qualifier : require

layout (location = 0) out vec4 outFragColor;

layout (location = 0) in vec3 inUV;

layout(set = 1, binding = 0) uniform samplerCube textures[];

void main()
{
    //10 is placeholder
    outFragColor = texture(textures[1], inUV);
}
