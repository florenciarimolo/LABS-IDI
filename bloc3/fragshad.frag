#version 330 core


in vec3 colorFocus;
in vec4 vertexSCO;
in mat3 nMatrix;
in vec3 normalSCO;
in mat4 viewMatrix;

out vec4 FragColor;

uniform vec3 posFocus;
 
// Propietats del material

in vec3 matambfrag;
in vec3 matdifffrag;
in vec3 matspecfrag;
in float matshinfrag;


vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    
    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambfrag;
    
    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
        colRes = colRes + colorFocus * matdifffrag * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats
    
    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);
    
    // Calculem R i V
    if (dot(NormSCO,L) < 0)
        return colRes;  // no hi ha component especular
    
    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);
    
    if ((dot(R, V) < 0) || (matshinfrag == 0))
        return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinfrag);
    return (colRes + matspecfrag * colorFocus * shine);
}


void main()
{
    vec4 focusSCO  = viewMatrix * vec4(posFocus, 1.0);
    vec4 L         = focusSCO - vertexSCO;
    vec3 Lxyz      = normalize(L.xyz);
    vec3 fcolor         = Phong (normalSCO, Lxyz, vertexSCO);
	FragColor = vec4(fcolor,1);	
}
