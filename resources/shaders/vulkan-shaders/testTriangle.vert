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

layout(buffer_reference, std430) readonly buffer VertexBuffer{
    Vertex vertices[];
};

layout( push_constant ) uniform constants
{
    mat4 render_matrix;
    VertexBuffer vertexBuffer;
} PushConstants;

void main()
{
    Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];

    gl_Position = PushConstants.render_matrix * vec4(v.position, 1.0f);
    //outColor = v.Color.xyz;
    outColor = vec3(1.0, 1.0, 1.0);
}
