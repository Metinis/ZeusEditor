#version 410 core
out vec4 fragColor;

in vec3 v_LocalPos;

uniform samplerCube u_EnvironmentMap;

void main()
{
    vec3 envColor = texture(u_EnvironmentMap, v_LocalPos).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    fragColor = vec4(envColor, 1.0);
}