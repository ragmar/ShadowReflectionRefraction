uniform vec3 eyePos;
uniform float fresnelBias, fresnelScale, fresnelPower;
uniform float etaRatio;
varying vec3 reflectedVector, refractedVector;
varying float redFactor;

void main() {
	vec3 i = normalize(gl_Vertex.xyz - eyePos.xyz);
	vec3 n = normalize(gl_Normal);
	reflectedVector = reflect(i, n);
	refractedVector = refract(i, n, etaRatio);
	redFactor = max(0.0, min(1.0, fresnelBias + fresnelScale * pow(1.0 + dot(i, n), fresnelPower)));
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
