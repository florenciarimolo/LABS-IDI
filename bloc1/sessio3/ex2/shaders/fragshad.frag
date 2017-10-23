#version 330 core

out vec4 FragColor;
in vec3 fcolors;

void main() {
    FragColor = vec4 (fcolors, 1.);
}

