uniform sampler2DRect tex0;
uniform float scale;

varying vec3 colour;


void main() {
	
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec2 st0 = gl_TexCoord[0].st;
	
	colour = vec3(texture2DRect(tex0, st0));
	
	vec4 pos = gl_Vertex;
	float rgb = (colour.r * 0.3) + (colour.g * 0.59) + (colour.b * 0.11);
	pos.z += rgb * scale;
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}
