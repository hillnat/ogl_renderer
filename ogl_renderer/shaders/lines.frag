#version 430 core
//To read from GPU buffer from vert shader
in vec4 vPos;
in vec4 vCol;
in vec2 vUV;
in vec3 vNorm;

//To output, the actual color
out vec4 fragColor;



void main(){
	fragColor = abs(vPos*0.5f);
}