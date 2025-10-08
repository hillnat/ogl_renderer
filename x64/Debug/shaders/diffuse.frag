#version 430 core
in vec4 vCol;
in vec2 vUV;
in float vTexLayer;
out vec4 FragColor;

uniform sampler2DArray diffuseTex;
uniform float fadeStart=0.999; // depth value where fade begins (e.g. 0.8)
uniform float fadeEnd=1;   // depth value where fully faded (e.g. 1.0)

void main() {
    vec4 texColor = texture(diffuseTex, vec3(vUV, vTexLayer));
    FragColor = texColor * vCol;

    if (FragColor.a < 0.01)
        discard;

    // Get depth in normalized device coordinates (0 = near, 1 = far)
    float depth = gl_FragCoord.z;

    // Compute fade factor
    float fade = clamp((fadeEnd - depth) / (fadeEnd - fadeStart), 0.0, 1.0);

    // Apply fade to alpha (or to color for stronger visual effect)
    FragColor.a *= fade;
}