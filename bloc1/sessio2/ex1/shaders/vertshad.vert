#version 330 core

in vec3 vertex;
in vec3 color;

void main()  {
    gl_Position = vec4 (vertex, 1.0);
}
