#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "glew/glew.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include "glfw/glfw3.h"
#include <unordered_map>
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;





struct Texture {
	GLuint handle;//OpenGL texture name
	unsigned width, height, channels;//image metadata
};



struct AnimationChannel {
	std::string nodeName;
	enum TargetType { Translation, Rotation, Scale } target;
	std::vector<double> times;
	std::vector<glm::vec3> vec3Values;   // for T/S
	std::vector<glm::quat> quatValues;   // for Rotation
};

struct Animation {
	std::string name;
	double duration;
	double ticksPerSecond;
	std::vector<AnimationChannel> channels;
};
struct vertex {
	glm::vec4 pos = glm::vec4(0, 0, 0, 1);
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	glm::vec3 normal = glm::vec3(0, 0, 0);
	glm::vec2 uv = glm::vec2(0, 0);
	float texLayer = 0.0f;  // NEW: texture layer index
	glm::ivec4 boneIDs = glm::ivec4(0);
	glm::vec4 boneWeights = glm::vec4(0.f);

	vertex() = default;

	// Non-skeletal constructor
	vertex(glm::vec4 p, glm::vec4 c, glm::vec3 n, glm::vec2 t, float tl = 0.0f)
		: pos(p), color(c), normal(n), uv(t), texLayer(tl), boneIDs(0), boneWeights(0.f) {
	}

	// Skeletal constructor
	vertex(glm::vec4 p, glm::vec4 c, glm::vec3 n, glm::vec2 t, glm::ivec4 bIDs, glm::vec4 bWeights, float tl = 0.0f)
		: pos(p), color(c), normal(n), uv(t), texLayer(tl), boneIDs(bIDs), boneWeights(bWeights) {
	}
};
struct BoneInfo {
	glm::mat4 offsetMatrix;   // inverse bind matrix (from glTF)
	glm::mat4 finalTransform; // updated each frame by animation
};
struct Mesh {
	// GPU handles
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint indexCount = 0;

	// CPU-side animation data
	std::vector<BoneInfo> bones;
	std::unordered_map<std::string, int> boneMap;
	std::vector<Animation> animations;

	std::unordered_map<std::string, glm::mat4> nodeDefaultTransforms;
	std::unordered_map<std::string, std::string> nodeParents;

	// flags
	bool hasSkinning() const { return !bones.empty(); }
	bool hasAnimations() const { return !animations.empty(); }
};

namespace Primitives {
	Mesh* MakeSphere();
	Mesh* MakePlane();
	GLuint CreateTextureArray(std::vector<Texture*> textures);
}

struct Shader {
	GLuint program;
};

struct Light {
	Light(vec3 color, vec3 direction) {
		this->color = color;
		this->direction = direction;
	}
	vec3 color = vec3(1.f, 1.f, 1.f);
	vec3 direction = vec3(1.f, 1.f, 1.f);
};
struct Material
{
	Material(Shader* shader) {
		this->shader = shader;
	}
	Shader* shader;
	/// <summary>
	/// Sets the texture of this material / shader
	/// </summary>
	/// <param name="tex">Texture to use</param>
	void SetTexture(Texture* tex) {
		//Renderer::SetUniform(shader, (GLuint)3, tex, 0);//Attach texture to shader
		textures.push_back(tex);
		texArr = Primitives::CreateTextureArray(textures);
	}
	void SetTexture(std::vector<Texture*> tex) {
		/*//GLuint location = glGetUniformLocation(shader->program, "diffuseTex");
		GLuint texArr = Renderer::CreateTextureArray(tex);
		for (int i = 0; i < tex.size(); i++) {
			Renderer::SetUniform(shader, (GLuint)3, texArr, (int)i); // Use texture handle and slot i
		}*/
		textures = tex;
		texArr = Primitives::CreateTextureArray(textures);
	}
	// Optional: store in a vector for dynamic usage
	std::vector<Texture*> textures;
	GLuint texArr;
};
const vec4 RED = vec4{ 1.f,0.f,0.f,1.f };
const vec4 BLUE = vec4{ 0.f,0.f,1.f,1.f };
const vec4 GREEN = vec4{ 0.f,1.f,0.f,1.f };
const vec4 YELLOW = vec4{ 1.f,1.f,0.f,1.f };
const vec4 MAGENTA = vec4{ 1.f,0.f,1.f,1.f };
const vec4 CYAN = vec4{ 0.f,1.f,1.f,1.f };
const vec4 GREY = vec4{ 0.5f,0.5f,0.5f,1.f };
const vec4 BLACK = vec4{ 1.f,0.f,0.f,1.f };
const vec4 WHITE = vec4{ 1.f,1.f,1.f,1.f };
