#pragma once
#include "glew/glew.h"
#include "glm/glm.hpp"//Math lib
#include <unordered_map>
#include <vector>
#include <string>
#include"../engine/Scene.h"
#include"../engine/GameObject.h"
#include"MeshImporter.h"

using std::string;
using std::vector;
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;
  

namespace Renderer {
	void ReportShaderCompileStatus(GLuint& shaderToReport);

	Mesh* MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount);//GLsizei = GL Size Index

	Mesh* MakeMesh(const vector<vertex> verts, const vector<GLuint> indices);

	void FreeMesh(Mesh* geo);

	void DrawMesh(const Shader* shader, const Mesh* mesh);

	Shader* MakeShader(const char* vertShader, const char* fragShader);

	Shader* MakeShader(const string& vertShader, const string& fragShader);

	Shader* LoadShader(const char* vertPath, const char* fragPath);

	void FreeShader(Shader* shader);

	void SetUniform(const Shader* shader, GLuint location, const mat4* value);
	void SetUniform(const Shader* shader, GLuint location, int count, const mat4* values);
	void SetUniform(const Shader* shader, GLuint location, const Texture* value, int textureSlot);
	void SetUniform(const Shader* shader, GLuint location, const GLuint textureArrayHandle, int textureSlot);
	void SetUniform(const Shader* shader, GLuint location, const vec4* value);
	void SetUniform(const Shader* shader, GLuint location, const vec3* value);
	void SetUniform(const Shader* shader, GLuint location, int count, const vec3* values);
	void SetUniform(const Shader* shader, GLuint location, const mat4 value);
	void SetUniform(const Shader* shader, GLuint location, const vec4 value);
	void SetUniform(const Shader* shader, GLuint location, const vec3 value);
	void SetUniform(const Shader* shader, GLuint location, int count, const vec3 value);
	void SetUniform(const Shader* shader, GLuint location, int count, const vec3* values);
	void DrawObjects(Scene* scene);



	Texture* MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);
	void FreeTexture(Texture* tex);
	Texture* LoadTexture(const char* imagePath);
	string ReadFile(const char* path);



}