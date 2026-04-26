#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outFragPos;
layout (location = 3) out vec2 outUV;

struct Vertex {
    vec3 position;
    float _pad0;
    vec3 Normal;
    float _pad1;
    vec2 TexCoords;
    float _pad2;
    float _pad3;
    vec4 Color;
    vec3 Tangent;
    float _pad4;
    vec3 Bitangent;
    float _pad5;
};

layout(set = 0, binding = 0) uniform SceneData {
    mat4 viewproj;
    vec4 ambientColor;
    vec4 u_LightPos; // w for sun power
    vec4 sunlightColor;
} SceneDataBuffer;

layout(buffer_reference, std430) readonly buffer VertexBufferAdr {
    Vertex vertices[];
};

layout(push_constant) uniform constants {
    mat4 u_ModelMat;
    VertexBufferAdr vertexBufferAdr;

    vec4 u_Albedo;   // xyz = color
    vec4 u_Params;   // x=metallic, y=roughness, z=ao, w=unused

    uint albedoIndex;
    /*uint metallicIndex;
    uint roughnessIndex;
    uint normalIndex;
    uint aoIndex;*/
} PerObjectData;

void main()
{
    Vertex v = PerObjectData.vertexBufferAdr.vertices[gl_VertexIndex];

    outFragPos = vec3(PerObjectData.u_ModelMat * vec4(v.position, 1.0f));
    gl_Position = SceneDataBuffer.viewproj * PerObjectData.u_ModelMat * vec4(v.position, 1.0f);
    outNormal = mat3(transpose(inverse(PerObjectData.u_ModelMat))) * v.Normal;

    //outColor = v.Color.xyz;
    //outAlbedoIdx = PerObjectData.albedoIndex;
    outUV = v.TexCoords;
    outColor = vec3(1.0, 1.0, 1.0);
}
