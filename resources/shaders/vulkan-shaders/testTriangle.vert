#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;

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
    mat4 view;
    mat4 proj;
    mat4 viewproj;
    vec4 ambientColor;
    vec4 sunlightDirection; // w for sun power
    vec4 sunlightColor;
} SceneDataBuffer;

layout(buffer_reference, std430) readonly buffer VertexBufferAdr {
    Vertex vertices[];
};

layout( push_constant ) uniform constants
{
    mat4 u_ModelMat;
    VertexBufferAdr vertexBufferAdr;
} PerObjectData;

void main()
{
    Vertex v = PerObjectData.vertexBufferAdr.vertices[gl_VertexIndex];

    gl_Position = SceneDataBuffer.viewproj * PerObjectData.u_ModelMat * vec4(v.position, 1.0f);
    //outColor = v.Color.xyz;
    outColor = vec3(1.0, 1.0, 1.0);
}
