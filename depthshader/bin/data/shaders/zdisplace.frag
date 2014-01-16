varying vec3 colour;

void main(void) {
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
	gl_FragColor = vec4(colour.rgb, 1.0);
}
