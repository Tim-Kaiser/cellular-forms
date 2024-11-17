#version 460

layout (location=0)in vec3 vertex;
layout (location=1)in vec3 normal;
layout (location=2)in vec2 uv;
layout (location=3) in vec3 p;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normalVec;
out vec3 vertexOut;

void main() {
   
    vertexOut = vec3(vec4(vertex + p,  1.));
    mat3 normalModel = transpose(inverse(mat3(model)));
    normalVec = normalModel * (normal + p);
    gl_Position = projection * view * model * vec4(vertexOut,  1.);
}