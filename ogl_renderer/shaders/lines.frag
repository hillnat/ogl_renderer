#version 430 core
//To read from GPU buffer from vert shader
in vec4 vCol;

//To output, the actual color
out vec4 fragColor;



void main(){

	fragColor = vCol;
}