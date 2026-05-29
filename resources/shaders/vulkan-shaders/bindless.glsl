#extension GL_EXT_buffer_reference : require
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 1, binding = 0) uniform sampler2D textures[];
layout(set = 1, binding = 0) uniform samplerCube cubeTextures[];

layout(set = 1, binding = 1, rgba8) uniform image2D rgba8Images[];
layout(set = 1, binding = 1, rgba16f) uniform image2D rgba16fImages[];

struct Material {
    vec4 u_Albedo;   // xyz = color
    vec4 u_Params;   // x=metallic, y=roughness, z=ao, w=unused

    uint albedoIndex;
    uint metallicIndex;
    uint roughnessIndex;
    uint normalIndex;
    uint aoIndex;

    uint flags;
};

layout(std430, set = 2, binding = 0) readonly buffer MaterialBuffer {
    Material materials[];
} materialBuffers;