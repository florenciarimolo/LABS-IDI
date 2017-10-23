#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4 (1.);
    if (gl_FragCoord .x < 347.) {
        if (gl_FragCoord .y > 347.)
            FragColor = vec4 (1. ,0. ,0. ,1.);   // vermell
        else FragColor = vec4 (1. ,1. ,0. ,1.);  // groc
    }
    else {
        if (gl_FragCoord .y > 347.)
            FragColor = vec4 (0. ,0. ,1. ,1.);   // blau
        else FragColor = vec4 (0. ,1. ,0. ,1.);  // verd
    }

}

