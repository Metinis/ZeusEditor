#version 410 core

uniform vec4 u_Color;
in vec4 vColor;    // interpolated color from vertex shader
out vec4 FragColor;

void main() {
    //FragColor = vColor;
    FragColor = u_Color;
}