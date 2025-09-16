#version 410 core
out vec4 fragColor;

in vec3 v_UV;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, v_UV);
}