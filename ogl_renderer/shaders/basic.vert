#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

//Uniforms
layout (location = 0) uniform mat4 proj; //clipspace
layout (location = 1) uniform mat4 view; //viewspace
layout (location = 2) uniform mat4 model; //worldspace ie transform

out vec4 vPos;
out vec4 vCol;
out vec2 vUV;
out vec3 vNorm;

void main(){
	vPos=model*position;
	vCol=color;
	vUV=uv;
	vNorm=mat3(model)*normal;
	gl_Position = proj*view * model * position;
}