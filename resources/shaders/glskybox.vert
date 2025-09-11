#version 410 core
layout (location = 0) in vec3 a_Position;

layout(std140) uniform View {
    mat4 u_VP;
};

out vec3 v_UV;

void main()
{
    v_UV = a_Position;
    vec4 pos = u_VP * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}
