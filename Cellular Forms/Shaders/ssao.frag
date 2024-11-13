#version 460

in vec2 vUv;
in vec3 norm;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gColor;


out vec4 fragCol;

void main() {
	vec3 color = texture(gColor, vUv).rgb;
	vec3 normal = texture(gNormal, vUv).rgb;
	vec3 pos = texture(gPos, vUv).rgb;

	fragCol = vec4( normal ,1.0);
}