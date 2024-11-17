#version 460

layout (location=0)in vec3 vertex;
layout (location=1)in vec3 normal;
layout (location=2)in vec2 uv;
 
out vec2 vUv;
out vec3 norm;

void main() {
    vUv = uv;
    norm = normalize(normal);
    gl_Position = vec4(vertex,  1.);
}