#version 460

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColor;

in vec3 vertexOut;
in vec3 normalVec;

out vec4 fragCol;

void main()
{    
    gPos = vertexOut;
    gNormal = normalize(normalVec);
    gColor = vec3(1.0);
}  
