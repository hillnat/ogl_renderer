#version 430 core

in vec4 vPos;
in vec4 vCol;
in vec2 vUV;
out vec4 fragColor;

layout (location = 3) uniform sampler2D mainTex;

void main(){
	//fragColor=vec4(1,1,1,1);
	fragColor=vec4(vec3(vCol),1.0);
	//fragColor = texture(mainTex, vUV)*vCol;
} 