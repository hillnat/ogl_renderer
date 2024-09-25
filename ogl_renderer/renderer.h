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
struct Vertex {
	vec4 pos;
	vec4 color;//Vertex color, usually unused or used for something else
	vec2 uv;
	vec3 normal;
};

struct Mesh {
	GLuint vao, vbo, ibo;//Vertex Array Object (This is made of the VBO and IBO), Vertex Buffer Object (All the vertices), Index Buffer Object (Index buffer holds the orders for vertices to be turned into tris)
	GLuint size = 0;//Length of the obj
};
struct Shader {
	GLuint program;
};
struct Texture {
	GLuint handle;//OpenGL texture name
	unsigned width, height, channels;//image metadata
};
struct Light {
	Light(vec3 a, vec3 b) {
		color = a;
		direction = b;
	}
	vec3 color = vec3(1.f, 1.f, 1.f);
	vec3 direction = vec3(1.f, 1.f, 1.f);
};
#pragma endregion
#pragma region Meshes
Mesh MakeMesh(const Vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount);//GLsizei = GL Size Index
//Geometry MakeGeometry(const vector<Vertex> verts, const vector<GLuint> indices);
void FreeMesh(Mesh& geo);
void DrawMesh(const Shader& shader, const Mesh& geo);
#pragma endregion
#pragma region Shader Functions
Shader MakeShader(const char* vertShader, const char* fragShader);
Shader MakeShader(const string& vertShader, const string& fragShader);
Shader LoadShader(const char* vertPath, const char* fragPath);
void FreeShader(Shader& shader);
#pragma endregion
#pragma region Uniform Functions
void SetUniform(const Shader& shader, GLuint location, const mat4& value);//For transforms
void SetUniform(const Shader& shader, GLuint location, const Texture& value, int textureSlot);
void SetUniform(const Shader& shader, GLuint location, const vec4& value);//For color and quaternions
void SetUniform(const Shader& shader, GLuint location, const vec3& value);//For rgb color and positions
void SetUniform(const Shader& shader, GLuint location, int count, const vec3& values);//For rgb color and positions
#pragma endregion
#pragma region Texture Functions
Texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);
void FreeTexture(Texture& tex);
Texture LoadTexture(const char* imagePath);
#pragma endregion
string ReadFile(const char* path);
string ReadFile(string path);


