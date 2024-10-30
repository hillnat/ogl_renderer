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
    //Higher specular power = smaller specular
    float specularPower = 0.001;
    vec3 normal = normalize(vNorm);
    vec3 lightDir = normalize(dirlightDirection);
    vec3 viewDir = normalize(cameraPos - vPos.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
    //Calculate the specular term
    float specularTerm = pow(max(0.0, dot(reflectDir, viewDir)), specularPower);
    //Calculate Lambertian diffuse term
    float lambertTerm = max(0.0, dot(normal, lightDir));
    vec4 diffuselight = vec4(lambertTerm * dirlightColor, 1.0);
    //Combine ambient light
    vec4 ambientlight = vec4(ambient.rgb, 1.0);
    //Calculate final texture color
    vec4 texColor = texture(mainTex, vUV);
    //texColor = clamp(texColor, 0.0, 1.0);

    //Calculate the final color
    fragColor = (texColor * diffuselight + ambientlight + vec4(dirlightColor * specularTerm, 1.0));
    //fragColor = clamp(fragColor, 0.0, 1.0);
    
    //fragColor = texColor;
}