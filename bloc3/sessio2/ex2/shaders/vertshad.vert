#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem dels focus de llum
uniform vec3 colFocus;
uniform vec3 posFocus;  // en SCO

out vec4 vertexSCO;
out vec3 NormSCO;
out vec4 focusSCO;

out vec3 matambfrag;
out vec3 matdifffrag;
out vec3 matspecfrag;
out float matshinfrag;
out vec3 colorFocus;

void main()
{	
      mat3 nMatrix   = inverse(transpose(mat3(view * TG)));
      NormSCO   = normalize(nMatrix * normal);
      vertexSCO = view * TG * vec4(vertex, 1.0);
      focusSCO  = view * vec4(posFocus, 1.0);
      colorFocus = colFocus;
      gl_Position = proj * view * TG * vec4 (vertex, 1.0);
      matambfrag = matamb;
      matdifffrag = matdiff;
      matspecfrag = matspec;
      matshinfrag = matshin;
}
