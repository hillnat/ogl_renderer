#version 430 core


layout (location = 3) uniform sampler2D mainTex;


in vec4 vPos;
in vec4 vCol;    
in vec2 vUV;
in vec3 vNorm;

out vec4 fragColor;

layout (location = 4) uniform vec3 ambient;
layout (location = 5) uniform vec3 dirlightColor;
layout (location = 6) uniform vec3 dirlightDirection;
layout (location = 7) uniform vec3 cameraPos;

void main()
{
    fragColor = clamp(vPos * vCol, vec3(0,0,0,0), vec3(1,1,1,1));
}