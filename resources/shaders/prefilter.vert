#version 410 core
layout (location = 0) in vec3 a_Position;

layout(std140) uniform View {
    mat4 u_VP;
};

out vec3 v_LocalPos;

void main()
{
    v_LocalPos = a_Position;

    vec4 clipPos = u_VP * vec4(v_LocalPos, 1.0);

    gl_Position = clipPos;
}