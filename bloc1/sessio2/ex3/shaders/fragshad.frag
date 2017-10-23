#version 330 core

out vec4 FragColor;
//in vec3 fcolors;
vec4 blanc = vec4(1.0, 1.0, 1.0, 1.0);
vec4 vermell = vec4 (1. ,0. ,0. ,1.);
vec4 groc = vec4 (1. ,1. ,0. ,1.);
vec4 blau = vec4 (0. ,0. ,1. ,1.);
vec4 verd = vec4 (0. ,1. ,0. ,1.);
void main() {
    FragColor = vec4 (blanc);
        if ( gl_FragCoord .x < 347.) {
            if (gl_FragCoord .y > 347.)
                FragColor = vermell;   // vermell
            else FragColor =  groc; // groc
        }
        else {
            if (gl_FragCoord .y > 347.)
                FragColor =  blau;  // blau
            else FragColor =  verd; // verd
        }
}

