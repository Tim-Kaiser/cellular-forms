#version 460

in vec2 vUv;
in vec3 norm;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gColor;


out vec4 fragCol;

void main() {
	vec3 pos = texture(gPos, vUv).xyz;
	vec3 color = texture(gColor, vUv).xyz;
	vec3 normal = texture(gNormal, vUv).xyz;

	//fragCol = vec4( vUv, 0.0 ,1.0);
	fragCol = vec4(normal , 1.0);
}