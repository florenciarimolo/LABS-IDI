#version 330 core
in vec3 vertex;
in vec3 color;
out vec3 fcolors;
uniform float escala;
uniform mat4 TG;

void main()
{
    fcolors = color;
    gl_Position = TG * vec4(escala * vertex, 1.0);
}
