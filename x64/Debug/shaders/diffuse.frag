#version 430 core

in vec4 vPos;
in vec4 vCol;
in vec2 vUV;
in vec3 vNorm;

out vec4 fragColor;

layout (location = 3) uniform sampler2D mainTex;
layout (location = 4) uniform vec3 ambient;
layout (location = 5) uniform vec3 dirLightColor;
layout (location = 6) uniform vec3 dirLightDirection;
layout (location = 7) uniform vec3 cameraPos;

void main()
{
    float specularPower = 64.0;
    vec3 n = normalize(vNorm);
    vec3 L = normalize(dirLightDirection);
    vec3 V = normalize(cameraPos - vPos.xyz);
    vec3 R = reflect(-L, n);
    
    float specularTermination = pow(max(0.0, dot(R, V)), specularPower);
    
    // Lambert term
    float lambert = max(0.0, dot(n, -L));
    vec4 lum = vec4(lambert, lambert, lambert, 1.0);

    vec4 baseLit = vec4(ambient.rgb, 1.0);
    vec4 litColor = vec4(dirLightColor, 1.0);
    vec4 specular = vec4(dirLightColor * specularTermination, 1.0);
    
    vec4 texColor = texture(mainTex, vUV) * ((litColor * lum));
    texColor = clamp(texColor, 0.0, 1.0);
    
    vec4 finalColor = texColor + specular;
    
    fragColor = clamp((vCol * finalColor)+(vCol * baseLit), 0.0, 1.0);
}
