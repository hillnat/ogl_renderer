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
layout (location = 5) uniform vec3 lightColor[3];
layout (location = 6) uniform vec3 lightDirection[3];
layout (location = 7) uniform vec3 cameraPos;


void main(){
	vec4 baseLit = vec4(ambient.rgb, 1.0);
	//float specularPower = 64.0;
	//vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 litColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 light = vec4(1.0, 1.0, 1.0, 1.0);
	//for(int i = 0; i<3; i++){
		//vec3 n = normalize(vNorm);
		//vec3 L = normalize(lightDirection[i]);
		//vec3 V = normalize(cameraPos - vPos.xyz);
		//vec3 R = reflect(L,n);
		//float specularTermination = pow(max(0,dot(R,V)), specularPower);
		//float lambert = max(0,dot(n,-L));	
		//light += vec4(lambert, lambert, lambert, 0.0);
		//litColor += vec4(lightColor[i],1.0);
		//specular += vec4(lightColor[i] * specularTermination,0.0);

	//}
	//litColor=normalize(litColor*vCol);
	//specular=normalize(specular);
	litColor=vCol;
	vec4 texColor = texture(mainTex, vUV) * litColor;
	//fragColor=texColor*light+specular;
	fragColor = texColor * light;
}