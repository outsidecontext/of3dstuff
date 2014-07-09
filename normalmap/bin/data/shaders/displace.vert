uniform sampler2D colormap;
uniform sampler2D bumpmap;
uniform sampler2D normalMap;
varying vec2  TexCoord;
uniform int maxHeight;

varying vec4 ecPosition; 
varying vec3 ecPosition3; 
varying vec3 normal;
varying vec4 ambientGlobal;

void main(void) {
    TexCoord = gl_MultiTexCoord0.st;

    vec4 bumpColor = texture2D(bumpmap, TexCoord);
    float df = 0.30*bumpColor.x + 0.59*bumpColor.y + 0.11*bumpColor.z;
    vec4 newVertexPos = vec4(gl_Normal * df * float(maxHeight), 0.0) + gl_Vertex;

    vec3 norm = normalize(texture2D(normalMap, TexCoord).rgb * 2. - 1.);

    ecPosition		= gl_ModelViewMatrix * newVertexPos; 
	ecPosition3		= vec3( ecPosition ) / ecPosition.w;
	//normal			= gl_NormalMatrix * gl_Normal;
    normal = (gl_NormalMatrix * gl_Normal) + norm;
	gl_Position		= gl_ModelViewProjectionMatrix * newVertexPos;
	ambientGlobal	= gl_LightModel.ambient * gl_FrontMaterial.ambient;

}