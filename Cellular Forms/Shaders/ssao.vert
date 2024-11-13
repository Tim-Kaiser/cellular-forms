#version 460

layout (location=0)in vec3 vertex;
layout (location=1)in vec2 uv;
layout (location=2)in vec3 normal;

 
out vec2 vUv;
out vec3 norm;

void main() {
    vec2 vUv = uv;
    norm = normal;
    gl_Position = vec4(vertex,  1.);
}