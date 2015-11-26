// Adapted from http://prideout.net/blog/?p=22

#define MAX_LIGHTS 8

uniform int numLights;

varying vec3 normal, eyeVec;
varying vec3 lightDir[MAX_LIGHTS];


float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}


void main()
{
	float Shininess = gl_FrontMaterial.shininess;
	vec3 N = normalize(normal);
    vec3 L = normalize(lightDir[0]);
    vec3 Eye = vec3(0, 0, 1);
    vec3 H = normalize(L + Eye);
    
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);

    const float A = 0.1;
    const float B = 0.3;
    const float C = 0.6;
    const float D = 1.0;
    float E = fwidth(df);

    if      (df > A - E && df < A + E) df = stepmix(A, B, E, df);
    else if (df > B - E && df < B + E) df = stepmix(B, C, E, df);
    else if (df > C - E && df < C + E) df = stepmix(C, D, E, df);
    else if (df < A) df = 0.0;
    else if (df < B) df = B;
    else if (df < C) df = C;
    else df = D;

    E = fwidth(sf);
    if (sf > 0.5 - E && sf < 0.5 + E)
    {
        sf = smoothstep(0.5 - E, 0.5 + E, sf);
    }
    else
    {
        sf = step(0.5, sf);
    }
    
    vec4 color = gl_FrontMaterial.ambient + df * gl_LightSource[0].diffuse + sf * gl_LightSource[0].specular;
    gl_FragColor = color;

}