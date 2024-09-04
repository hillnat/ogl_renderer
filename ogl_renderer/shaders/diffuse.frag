#version 430 core
//To read from GPU buffer from vert shader
in vec4 vPos;
in vec4 vCol;
in vec2 vUV;
in vec3 vNorm;
//To output, the actual color
out vec4 fragColor;

//Uniforms are global variables
layout (location = 3) uniform sampler2D mainTex;
layout (location = 4) uniform vec3 ambient;
layout (location = 5) uniform vec3 dirLightColor;
layout (location = 6) uniform vec3 dirLightDirection;
layout (location = 7) uniform vec3 cameraPos;


void main(){
	vec4 baseLit = vec4(ambient.rgb, 1.0);
	
	vec3 n = normalize(vNorm);
	vec3 L = normalize(dirLightDirection);
	
	float lambert = max(0,dot(n,-L));
	
	vec4 lum = vec4(lambert, lambert, lambert, 1.0);
	//Normalize vector to camera
	vec3 V = normalize(cameraPos - vPos.xyz);
	vec3 R = reflect(L,n);
	
	//specular power
	float specularPower = 64.0;
	float specularTermination = pow(max(0,dot(R,V)), specularPower);
	vec4 specular = vec4(dirLightColor * specularTermination,0);
	
	vec4 litColor = vec4(dirLightColor,1.0) * vCol;
	vec4 texColor = texture(mainTex, vUV) * litColor;
	
	fragColor=texColor*lum+specular;
}