#version 460

in vec3 vertex;
in vec2 uv;
in vec3 normal;
uniform mat4 projectionMatrix;

out vec3 normalVec;
out vec2 vUv;
out vec3 vertexOut;

void main() {
	gl_Position = projectionMatrix *  vec4(vertex, 1.0);

	vUv = uv;
	normalVec = normal;
	vertexOut = vec3(uv, 0.);
}