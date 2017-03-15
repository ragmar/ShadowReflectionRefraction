uniform samplerCube env;
varying vec3 reflectedVector, refractedVector;
varying float redFactor;

void main() {
	vec4 reflectedColor = textureCube(env, reflectedVector);
	vec4 refractedColor = textureCube(env, refractedVector);
	gl_FragColor = mix(refractedColor, reflectedColor, redFactor);
	gl_FragColor.a = 0.8;
}
