#include "MeshImporter.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glfw/glfw3.h"
#include <iostream>
#include <functional> 
#include <string>
#include <vector>
#include "stb/stb_image.h"
#include <set> 
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
using glm::mat4;
using glm::quat;
using glm::vec4;
using glm::vec3;
using glm::vec2;
#define MAX_BONES 150
int MeshImporter::animationIndex = 0;
namespace MeshImporter {
    // Helper to convert assimp matrix to glm
    glm::mat4 AssimpToGlm(const aiMatrix4x4& from) {
        glm::mat4 to;
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
    void BuildNodeHierarchy(const aiNode* node,
        std::unordered_map<std::string, glm::mat4>& nodeDefaultTransforms,
        std::unordered_map<std::string, std::string>& nodeParents,
        const std::string& parentName = "") {
        std::string nodeName = node->mName.C_Str();
        nodeDefaultTransforms[nodeName] = AssimpToGlm(node->mTransformation);

        if (!parentName.empty()) {
            nodeParents[nodeName] = parentName;
        }

        // Recursively process children
        for (unsigned i = 0; i < node->mNumChildren; ++i) {
            BuildNodeHierarchy(node->mChildren[i], nodeDefaultTransforms, nodeParents, nodeName);
        }
    }
    Mesh* ImportFromFileGlb(const char* filename)//Import GLB file
    {
        float startTime = glfwGetTime();

        const aiScene* scene = aiImportFile(filename,
            aiProcess_Triangulate |
            aiProcess_GenNormals |
            aiProcess_JoinIdenticalVertices |
            aiProcess_LimitBoneWeights
        );

        if (!scene || !scene->HasMeshes()) {
            std::cerr << "Failed to load mesh: " << filename << std::endl;
            return nullptr;
        }

        std::vector<vertex> allVertices;
        std::vector<GLuint> allIndices;
        std::unordered_map<std::string, int> boneMap;
        std::vector<BoneInfo> bones;
        int totalVertices = 0;

        //Process all meshes and build bone data
        for (unsigned m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            // Process bones first to build the boneMap
            if (mesh->HasBones()) {
                for (unsigned b = 0; b < mesh->mNumBones; ++b) {
                    aiBone* bone = mesh->mBones[b];
                    std::string boneName = bone->mName.C_Str();

                    int boneIndex = -1;

                    // Check if we've seen this bone before
                    if (boneMap.find(boneName) == boneMap.end()) {
                        // New bone
                        boneIndex = bones.size();
                        boneMap[boneName] = boneIndex;

                        BoneInfo boneInfo;
                        boneInfo.offsetMatrix = MeshImporter::AssimpToGlm(bone->mOffsetMatrix);
                        boneInfo.finalTransform = glm::mat4(1.0f);
                        bones.push_back(boneInfo);
                    }
                    else {
                        boneIndex = boneMap[boneName];
                    }
                }
            }

            // Indices
            for (unsigned f = 0; f < mesh->mNumFaces; ++f) {
                const aiFace& face = mesh->mFaces[f];
                for (unsigned idx = 0; idx < face.mNumIndices; ++idx) {
                    allIndices.push_back(face.mIndices[idx] + totalVertices);
                }
            }

            // Vertices
            for (unsigned v = 0; v < mesh->mNumVertices; ++v) {
                vertex vert;

                // Position
                vert.pos = glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);

                // Color
                if (mesh->HasVertexColors(0)) {
                    aiColor4D c = mesh->mColors[0][v];
                    vert.color = glm::vec4(c.r, c.g, c.b, c.a);
                }
                else {
                    vert.color = glm::vec4(1.0f);
                }

                // Normal
                if (mesh->HasNormals()) {
                    vert.normal = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
                }

                // UV
                if (mesh->HasTextureCoords(0)) {
                    vert.uv = glm::vec2(mesh->mTextureCoords[0][v].x, 1.f - mesh->mTextureCoords[0][v].y);
                }

                vert.texLayer = static_cast<float>(m); // m is the mesh index from the outer loop


                // Initialize bone data
                vert.boneIDs = glm::ivec4(0);
                vert.boneWeights = glm::vec4(0.f);

                // Assign bone weights
                if (mesh->HasBones()) {
                    int weightIndex = 0;
                    for (unsigned b = 0; b < mesh->mNumBones; ++b) {
                        aiBone* bone = mesh->mBones[b];
                        std::string boneName = bone->mName.C_Str();
                        int boneIndex = boneMap[boneName];

                        for (unsigned w = 0; w < bone->mNumWeights; ++w) {
                            if (bone->mWeights[w].mVertexId == v) {
                                if (weightIndex < 4) {
                                    vert.boneIDs[weightIndex] = boneIndex;
                                    vert.boneWeights[weightIndex] = bone->mWeights[w].mWeight;
                                    weightIndex++;
                                }
                            }
                        }
                    }

                    // Normalize weights to sum to 1.0
                    float totalWeight = vert.boneWeights.x + vert.boneWeights.y +
                        vert.boneWeights.z + vert.boneWeights.w;
                    if (totalWeight > 0.0f) {
                        vert.boneWeights /= totalWeight;
                    }
                }

                allVertices.push_back(vert);
            }

            totalVertices += mesh->mNumVertices;
        }


        Mesh* resultMesh = MeshImporter::MakeMesh(allVertices.data(), allVertices.size(),
            allIndices.data(), allIndices.size());

        // Store bone data
        resultMesh->bones = bones;
        resultMesh->boneMap = boneMap;


        if (scene->HasAnimations()) {
            std::cout << "Found " << scene->mNumAnimations << " animations\n";

            for (unsigned a = 0; a < scene->mNumAnimations; ++a) {
                aiAnimation* aiAnim = scene->mAnimations[a];

                Animation anim;
                anim.name = aiAnim->mName.C_Str();
                anim.duration = aiAnim->mDuration;
                anim.ticksPerSecond = aiAnim->mTicksPerSecond != 0 ? aiAnim->mTicksPerSecond : 25.0;

                std::cout << "  Animation: " << anim.name << " (" << anim.duration << " ticks)\n";

                // Process each channel (bone animation)
                for (unsigned c = 0; c < aiAnim->mNumChannels; ++c) {
                    aiNodeAnim* nodeAnim = aiAnim->mChannels[c];
                    std::string nodeName = nodeAnim->mNodeName.C_Str();

                    // Position keys
                    if (nodeAnim->mNumPositionKeys > 0) {
                        AnimationChannel posChannel;
                        posChannel.nodeName = nodeName;
                        posChannel.target = AnimationChannel::Translation;

                        for (unsigned k = 0; k < nodeAnim->mNumPositionKeys; ++k) {
                            posChannel.times.push_back(nodeAnim->mPositionKeys[k].mTime);
                            aiVector3D pos = nodeAnim->mPositionKeys[k].mValue;
                            posChannel.vec3Values.push_back(glm::vec3(pos.x, pos.y, pos.z));
                        }
                        anim.channels.push_back(posChannel);
                    }

                    // Rotation keys
                    if (nodeAnim->mNumRotationKeys > 0) {
                        AnimationChannel rotChannel;
                        rotChannel.nodeName = nodeName;
                        rotChannel.target = AnimationChannel::Rotation;

                        for (unsigned k = 0; k < nodeAnim->mNumRotationKeys; ++k) {
                            rotChannel.times.push_back(nodeAnim->mRotationKeys[k].mTime);
                            aiQuaternion rot = nodeAnim->mRotationKeys[k].mValue;
                            rotChannel.quatValues.push_back(glm::quat(rot.w, rot.x, rot.y, rot.z));
                        }
                        anim.channels.push_back(rotChannel);
                    }

                    // Scale keys
                    if (nodeAnim->mNumScalingKeys > 0) {
                        AnimationChannel scaleChannel;
                        scaleChannel.nodeName = nodeName;
                        scaleChannel.target = AnimationChannel::Scale;

                        for (unsigned k = 0; k < nodeAnim->mNumScalingKeys; ++k) {
                            scaleChannel.times.push_back(nodeAnim->mScalingKeys[k].mTime);
                            aiVector3D scale = nodeAnim->mScalingKeys[k].mValue;
                            scaleChannel.vec3Values.push_back(glm::vec3(scale.x, scale.y, scale.z));
                        }
                        anim.channels.push_back(scaleChannel);
                    }
                }

                resultMesh->animations.push_back(anim);
                std::cout << "    Loaded " << anim.channels.size() << " channels\n";
            }
        }
        else {
            std::cout << "No animations found in file\n";
        }

        //Build node hierarchy for skeletal animation
        if (scene->mRootNode) {
            std::cout << "Building node hierarchy from root: " << scene->mRootNode->mName.C_Str() << "\n";
            MeshImporter::BuildNodeHierarchy(scene->mRootNode, resultMesh->nodeDefaultTransforms, resultMesh->nodeParents);
            std::cout << "Built hierarchy with " << resultMesh->nodeDefaultTransforms.size() << " nodes\n";
        }
        else {
            std::cout << "WARNING: No root node found!\n";
        }

        float endTime = glfwGetTime();
        std::cout << "Loaded " << totalVertices << " vertices, "
            << bones.size() << " bones, "
            << resultMesh->animations.size() << " animations in "
            << endTime - startTime << "s.\n";

        aiReleaseImport(scene);
        return resultMesh;
    }

    int FindKeyframe(const std::vector<double>& times, double time) {
        for (int i = 0; i < times.size() - 1; i++) {
            if (time < times[i + 1]) return i;
        }
        return std::max(0, (int)times.size() - 1);
    }

    glm::vec3 InterpolateVec3(const AnimationChannel& channel, double time) {
        if (channel.vec3Values.empty()) return glm::vec3(0);
        if (channel.vec3Values.size() == 1) return channel.vec3Values[0];

        int idx = MeshImporter::FindKeyframe(channel.times, time);
        if (idx >= (int)channel.vec3Values.size() - 1) return channel.vec3Values.back();

        double t0 = channel.times[idx];
        double t1 = channel.times[idx + 1];
        float factor = (float)((time - t0) / (t1 - t0));
        factor = glm::clamp(factor, 0.0f, 1.0f);

        return glm::mix(channel.vec3Values[idx], channel.vec3Values[idx + 1], factor);
    }

    glm::quat InterpolateQuat(const AnimationChannel& channel, double time) {
        if (channel.quatValues.empty()) return glm::quat(1, 0, 0, 0);
        if (channel.quatValues.size() == 1) return channel.quatValues[0];

        int idx = MeshImporter::FindKeyframe(channel.times, time);
        if (idx >= (int)channel.quatValues.size() - 1) return channel.quatValues.back();

        double t0 = channel.times[idx];
        double t1 = channel.times[idx + 1];
        float factor = (float)((time - t0) / (t1 - t0));
        factor = glm::clamp(factor, 0.0f, 1.0f);

        return glm::slerp(channel.quatValues[idx], channel.quatValues[idx + 1], factor);
    }

    void UpdateAnimation(Mesh* mesh, double currentTime, std::vector<glm::mat4>& outBoneTransforms) {
        if (!mesh || mesh->animations.empty() || mesh->bones.empty()) {
            return;
        }
        if (animationIndex < 0) { animationIndex = mesh->animations.size() - 1; }
        else if (animationIndex >= mesh->animations.size()) { animationIndex = 0; }
        Animation& anim = mesh->animations[animationIndex];
        double animTime = fmod(currentTime * anim.ticksPerSecond, anim.duration);

        outBoneTransforms.clear();
        outBoneTransforms.resize(mesh->bones.size(), glm::mat4(1.0f));

        // Get node hierarchy
        auto& nodeDefaults = mesh->nodeDefaultTransforms;
        auto& nodeParents = mesh->nodeParents;

        if (nodeDefaults.empty()) {
            std::cout << "WARNING: No node hierarchy! Using simple animation.\n";
            // Fallback to simple animation without hierarchy
            for (const auto& pair : mesh->boneMap) {
                outBoneTransforms[pair.second] = mesh->bones[pair.second].offsetMatrix;
            }
            return;
        }

        // Build animated local transforms
        std::unordered_map<std::string, glm::mat4> localTransforms;

        // Start with defaults
        localTransforms = nodeDefaults;

        // Override with animation data
        std::unordered_map<std::string, glm::vec3> animTranslations;
        std::unordered_map<std::string, glm::quat> animRotations;
        std::unordered_map<std::string, glm::vec3> animScales;

        for (const auto& channel : anim.channels) {
            switch (channel.target) {
            case AnimationChannel::Translation:
                animTranslations[channel.nodeName] = InterpolateVec3(channel, animTime);
                break;
            case AnimationChannel::Rotation:
                animRotations[channel.nodeName] = InterpolateQuat(channel, animTime);
                break;
            case AnimationChannel::Scale:
                animScales[channel.nodeName] = InterpolateVec3(channel, animTime);
                break;
            }
        }

        // Build local transform for animated nodes
        for (const auto& pair : animTranslations) {
            glm::vec3 t = pair.second;
            glm::quat r = animRotations.count(pair.first) ? animRotations[pair.first] : glm::quat(1, 0, 0, 0);
            glm::vec3 s = animScales.count(pair.first) ? animScales[pair.first] : glm::vec3(1);

            localTransforms[pair.first] = glm::translate(glm::mat4(1.0f), t) *
                glm::toMat4(r) *
                glm::scale(glm::mat4(1.0f), s);
        }

        // Compute global transforms
        std::unordered_map<std::string, glm::mat4> globalTransforms;

        // Helper to recursively compute global transform
        std::function<glm::mat4(const std::string&)> computeGlobal;
        computeGlobal = [&](const std::string& nodeName) -> glm::mat4 {
            // Already computed?
            if (globalTransforms.count(nodeName)) {
                return globalTransforms[nodeName];
            }

            glm::mat4 local = localTransforms.count(nodeName) ?
                localTransforms[nodeName] : glm::mat4(1.0f);

            // Has parent?
            if (nodeParents.count(nodeName)) {
                glm::mat4 parentGlobal = computeGlobal(nodeParents[nodeName]);
                globalTransforms[nodeName] = parentGlobal * local;
            }
            else {
                // Root node
                globalTransforms[nodeName] = local;
            }

            return globalTransforms[nodeName];
            };

        // Apply to all bones
        for (const auto& pair : mesh->boneMap) {
            const std::string& boneName = pair.first;
            int boneIdx = pair.second;

            if (boneIdx < 0 || boneIdx >= (int)mesh->bones.size()) continue;

            glm::mat4 globalTransform = computeGlobal(boneName);
            outBoneTransforms[boneIdx] = globalTransform * mesh->bones[boneIdx].offsetMatrix;
        }
    }

    std::vector<Texture*> MeshImporter::ExtractTexturesFromFileGlb(const char* filepath) {
        std::vector<Texture*> textures;

        // Load the GLB file with Assimp
        const aiScene* scene = aiImportFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene) {
            std::cerr << "Failed to load GLB: " << filepath << std::endl;
            return textures;
        }

        // Iterate all materials
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            aiMaterial* material = scene->mMaterials[i];

            aiTextureType types[] = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS, aiTextureType_HEIGHT };

            for (auto type : types) {
                unsigned int count = material->GetTextureCount(type);
                for (unsigned int t = 0; t < count; t++) {
                    aiString texPath;
                    if (material->GetTexture(type, t, &texPath) != AI_SUCCESS) continue;

                    const aiTexture* embeddedTex = scene->GetEmbeddedTexture(texPath.C_Str());
                    if (!embeddedTex) continue;

                    if (embeddedTex->mHeight == 0) {
                        // Compressed image (PNG/JPG)
                        int width, height, channels;
                        unsigned char* pixels = stbi_load_from_memory(
                            reinterpret_cast<const unsigned char*>(embeddedTex->pcData),
                            embeddedTex->mWidth,
                            &width, &height, &channels, 4
                        );
                        if (pixels) {
                            textures.push_back(MeshImporter::MakeTexture(width, height, 4, pixels));
                            stbi_image_free(pixels);
                        }
                    }
                    else {
                        // Raw RGBA
                        textures.push_back(MeshImporter::MakeTexture(
                            embeddedTex->mWidth,
                            embeddedTex->mHeight,
                            4,
                            reinterpret_cast<const unsigned char*>(embeddedTex->pcData)
                        ));
                    }
                }
            }
        }

        // Free the scene
        aiReleaseImport(scene);

        return textures;
    }



    Mesh* MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount)
    {
        Mesh* mesh = new Mesh();
        mesh->indexCount = indexCount;
        glGenVertexArrays(1, &mesh->vao);
        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ibo);
        glBindVertexArray(mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
        glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(vertex), verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

        // UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));

        // Normal
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

        // Bone IDs (integer attribute)
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 4, GL_INT, sizeof(vertex), (void*)offsetof(vertex, boneIDs));

        // Bone weights
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, boneWeights));

        // Texture Layer (NEW!)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texLayer));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return mesh;
    }
    Mesh* MakeMesh(const std::vector<vertex> verts, const std::vector<GLuint> indices) {//Overload just calls the same func above with different data
        return MakeMesh(verts.data(), verts.size(), indices.data(), indices.size());
    }

    Texture* MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels) {
        GLenum oglFormat = GL_RGBA;
        switch (channels) {
        case 1:
            oglFormat = GL_RED;
            break;
        case 2:
            oglFormat = GL_RG;
            break;
        case 3:
            oglFormat = GL_RGB;
            break;
        default:
            oglFormat = GL_RGBA;
            break;
        }
        Texture* texObj = new Texture{ 0, width, height, channels };
        glGenTextures(1, &(texObj->handle));
        //bind and buffer texture
        glBindTexture(GL_TEXTURE_2D, (texObj->handle));
        glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);
        //Configure texture settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //Unbind and return the object
        glBindTexture(GL_TEXTURE_2D, 0);
        return texObj;
    }

    void FreeTexture(Texture* tex) {
        glDeleteTextures(1, &(tex->handle));
        tex = {};
    }
    Texture* LoadTexture(const char* imagePath) {
        //Setup varaibles to store texture data
        int imageWidth = -1;//-1 for error handling
        int imageHeight = -1;
        int imageFormat = -1;
        unsigned char* imagePixels = nullptr;
        Texture* newtexture = new Texture();
        stbi_set_flip_vertically_on_load(true);//Load using OpenGLConventions
        imagePixels = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);
        //Note : stdi_load will return a nullptr if it failed to load
        //Pass the data to OpenGL
        newtexture = MeshImporter::MakeTexture(imageWidth, imageHeight, imageFormat, imagePixels);
        stbi_image_free(imagePixels);
        imagePixels = nullptr;
        return newtexture;
    }
}