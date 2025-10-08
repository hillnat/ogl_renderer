#version 430 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;
layout (location = 4) in ivec4 boneIDs;
layout (location = 5) in vec4 boneWeights;
layout (location = 6) in float texLayer; // NEW: texture layer input

layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 model;
layout (location = 3) uniform mat4 bones[150];

out vec4 vPos;
out vec4 vCol;
out vec2 vUV;
out vec3 vNorm;
out float vTexLayer; // NEW: texture layer output (flat to avoid interpolation)

void main() {
    float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
    
    vec4 finalPos;
    vec3 finalNorm;
    
    if (totalWeight > 0.0) {
        mat4 skinMatrix = mat4(0.0);
        for (int i = 0; i < 4; ++i) {
            if (boneWeights[i] > 0.0 && boneIDs[i] < 150) {
                skinMatrix += bones[boneIDs[i]] * boneWeights[i];
            }
        }
        
        // Check if skinMatrix is valid (not all zeros)
        if (skinMatrix[0][0] != 0.0 || skinMatrix[1][1] != 0.0 || skinMatrix[2][2] != 0.0) {
            finalPos = skinMatrix * position;
            // Use the inverse transpose for normals (approximation: just use the 3x3 part)
            finalNorm = normalize(mat3(skinMatrix) * normal);
        } else {
            // Skinning matrix is invalid, use original position
            finalPos = position;
            finalNorm = normal;
        }
    } else {
        finalPos = position;
        finalNorm = normal;
    }
    
    // Transform to world space
    vPos = model * finalPos;
    vNorm = normalize(mat3(model) * finalNorm);
    vCol = color;
    vUV = uv;
    vTexLayer = texLayer; // NEW: pass texture layer to fragment shader
    
    gl_Position = proj * view * vPos;
}