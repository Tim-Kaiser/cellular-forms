#version 460

in vec2 vUv;
in vec3 vertexOut;
in vec3 normalVec;

out vec4 fragCol;

void main() {
	//fragCol = vec4(1., 0., 0., 1.);
	fragCol = vec4(normalVec, 1.);
}