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
uniform vec3 llumAmbient;

out vec4 vertexSCO;
out mat3 nMatrix;
out vec3 normalSCO;
out vec3 matambfrag;
out vec3 matdifffrag;
out vec3 matspecfrag;
out float matshinfrag;
out vec4 focusSCO;


void main()
{	
      focusSCO  = view * vec4(posFocus, 1.0);
      nMatrix   = inverse(transpose(mat3(view * TG)));
      normalSCO = normalize(nMatrix * normal);
      vertexSCO = view * TG * vec4(vertex, 1.0);
      gl_Position = proj * view * TG * vec4 (vertex, 1.0);
      matambfrag = matamb;
      matdifffrag = matdiff;
      matspecfrag = matspec;
      matshinfrag = matshin;
}
