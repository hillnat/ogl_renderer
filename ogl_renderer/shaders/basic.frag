#version 430 core
in vec4 vCol;
in vec2 vUV;        // original UVs (ignored for scaling)
in float vTexLayer;
out vec4 FragColor;

uniform sampler2DArray diffuseTex;
uniform float fadeStart = 0.999;
uniform float fadeEnd = 1.0;

// Controls how many world units per texture repeat
uniform float texScale = 1.0;

// Object bounding box (set per model in CPU code)
uniform vec3 objMin;
uniform vec3 objMax;

void main() {
    // Compute object-space position from normalized UV
    vec3 objPos = objMin + vec3(vUV, 0.0) * (objMax - objMin);

    // Use XZ plane for texture mapping
    vec2 worldUV = objPos.xz * texScale;

    vec4 texColor = texture(diffuseTex, vec3(worldUV, vTexLayer));
    FragColor = texColor * vCol;

    if (FragColor.a < 0.01)
        discard;

    // Depth-based fade
    float depth = gl_FragCoord.z;
    float fade = clamp((fadeEnd - depth) / (fadeEnd - fadeStart), 0.0, 1.0);
    FragColor.a *= fade;
}
