#version 460

in vec2 vUv;
in vec3 vertexOut;
layout(location = 0) out vec4 fragCol;

void main() {
	vec2 xy = vUv - 0.5;
	fragCol = vec4(xy, 0., 1.);
}