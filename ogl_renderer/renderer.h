#pragma once
#include "glew/glew.h"
#include "glm/glm.hpp"//Math lib
#include <vector>
#include <string>
using std::string;
using std::vector;
using glm::mat4;
using glm::vec4;
using glm::vec3;
using glm::vec2;
   
#pragma region Struct Definitions
struct vertex {
	~vertex(){}
	vertex() {
		this->pos = vec4(0, 0, 0, 1);
		this->color = vec4(1, 1, 1, 1);
		this->normal = vec3(0, 0, 0);
		this->uv = vec2(0, 0);
	}
	vertex(vec4 pos, vec4 color, vec3 normal, vec2 uv) {
		this->pos = pos;
		this->color = color;
		this->normal = normal;
		this->uv = uv;
	}
	vec4 pos = vec4(0, 0, 0, 1);
	vec4 color = vec4(1, 1, 1, 1);//vertex color, usually unused or used for something else
	vec3 normal = vec3(0, 0, 0);
	vec2 uv = vec2(0, 0);
};
struct mesh {
	GLuint vao, vbo, ibo;//vertex Array Object (This is made of the VBO and IBO), vertex Buffer Object (All the vertices), Index Buffer Object (Index buffer holds the orders for vertices to be turned into tris)
	GLuint size = 0;//Length of the obj
};
struct shader {
	GLuint program;
};
struct texture {
	GLuint handle;//OpenGL texture name
	unsigned width, height, channels;//image metadata
};
struct light {
	light(vec3 color, vec3 direction) {
		this->color = color;
		this->direction = direction;
	}
	vec3 color = vec3(1.f, 1.f, 1.f);
	vec3 direction = vec3(1.f, 1.f, 1.f);
};
#pragma endregion
#pragma region Meshes
mesh MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, const GLsizei indexCount);//GLsizei = GL Size Index
mesh MakeMesh(const vector<vertex> verts, const vector<GLuint> indices);
//Geometry MakeGeometry(const vector<vertex> verts, const vector<GLuint> indices);
void FreeMesh(mesh& geo);
void DrawMesh(const shader& shader, const mesh& geo);
#pragma endregion
#pragma region shader Functions
shader MakeShader(const char* vertshader, const char* fragshader);
shader MakeShader(const string& vertshader, const string& fragshader);
shader LoadShader(const char* vertPath, const char* fragPath);
void FreeShader(shader& shader);
#pragma endregion
#pragma region Uniform Functions
void SetUniform(const shader& shader, GLuint location, const mat4& value);//For transforms
void SetUniform(const shader& shader, GLuint location, const texture& value, int textureSlot);
void SetUniform(const shader& shader, GLuint location, const vec4& value);//For color and quaternions
void SetUniform(const shader& shader, GLuint location, const vec3& value);//For rgb color and positions
void SetUniform(const shader& shader, GLuint location, int count, const vec3& values);//For rgb color and positions arrays
#pragma endregion
#pragma region texture Functions
texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);
void FreeTexture(texture& tex);
texture LoadTexture(const char* imagePath);
#pragma endregion
string ReadFile(const char* path);
string ReadFile(string path);

