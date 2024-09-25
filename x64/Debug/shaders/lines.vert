#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

//Uniforms
layout (location = 0) uniform mat4 proj; //clipspace
layout (location = 1) uniform mat4 view; //viewspace
layout (location = 2) uniform mat4 model; //worldspace ie transform

out vec4 vCol;


void main(){
	vPos=model * position;
	vCol=color;
	//Model view projection matrix
	gl_Position = proj * view * model * position;
}