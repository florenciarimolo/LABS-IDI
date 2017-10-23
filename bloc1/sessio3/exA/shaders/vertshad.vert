#version 330 core
in vec3 vertex;
in vec3 color;
out vec3 fcolors;
uniform float escala;

void main()
{
fcolors = color;
gl_Position = vec4( escala * vertex, 1.0);
}
