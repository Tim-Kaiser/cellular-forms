#version 460

in vec2 vUv;
in vec3 vertexOut;
in vec3 normalVec;
in int InstanceID;

out vec4 fragCol;

void main() {
	//fragCol = vec4(0., 1., 1., 1.);
	vec3 c = vec3(0.7,0.7,0.7);
	fragCol = vec4(vUv, 1.0, 1.0);
}