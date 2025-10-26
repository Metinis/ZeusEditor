#version 410 core
layout (location = 0) in vec3 a_Position;

out vec3 v_LocalPos;

layout(std140) uniform View {
    mat4 u_VP;
};

void main()
{
    v_LocalPos = a_Position;
    gl_Position =  u_VP * vec4(v_LocalPos, 1.0);
}