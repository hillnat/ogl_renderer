#pragma once
#include "glm/glm.hpp"//Math lib
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "glew/glew.h"
#include "../engine/Primitives.h"
#include <vector>

namespace MeshImporter {

	glm::mat4 AssimpToGlm(const aiMatrix4x4& from);

	Mesh* ImportFromFileGlb(const char* filePath);

	void UpdateAnimation(Mesh* mesh, double currentTime, std::vector<glm::mat4>& outBoneTransforms);
	std::vector<Texture*> ExtractTexturesFromFileGlb(const char* filepath);
	glm::vec3 InterpolateVec3(const AnimationChannel& channel, double time);
	glm::quat InterpolateQuat(const AnimationChannel& channel, double time);
	void BuildNodeHierarchy(const aiNode* node,
		std::unordered_map<std::string, glm::mat4>& nodeDefaultTransforms,
		std::unordered_map<std::string, std::string>& nodeParents,
		const std::string& parentName);
	int FindKeyframe(const std::vector<double>& times, double time);
	extern int animationIndex; // declare as extern

	Mesh* MakeMesh(const std::vector<vertex> verts, const std::vector<GLuint> indices);
	Mesh* MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount);




	Texture* MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);

	void FreeTexture(Texture* tex);
	Texture* LoadTexture(const char* imagePath);
}
