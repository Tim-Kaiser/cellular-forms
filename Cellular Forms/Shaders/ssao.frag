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

	vec3 viewPos = vec3(0.0, 0.0, 2.0);
	vec3 viewDir = viewPos - pos;


	vec3 lightPos = vec3(0.0, 0.0, 100.0);
	vec3 lightDir = normalize(lightPos - pos);

	vec3 lightCol = vec3(1.0);

	float distToLight = abs(distance(lightPos, pos));

	vec3 col = color * 0.1;
	col += max(dot(normal, lightDir), 0.0) * lightCol * color ;

	//fragCol = vec4( vUv, 0.0 ,1.0);
	fragCol = vec4(col , 1.0);
}