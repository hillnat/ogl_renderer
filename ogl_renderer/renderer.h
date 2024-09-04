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

struct Geometry {
	GLuint vao, vbo, ibo;//Vertex Array Object (This is made of the VBO and IBO), Vertex Buffer Object (All the vertices), Index Buffer Object (Index buffer holds the orders for vertices to be turned into tris)
	GLuint size;//Length of the obj
};
struct Shader {
	GLuint program;
};
struct Texture {
	GLuint handle;//OpenGL texture name
	unsigned width, height, channels;//image metadata
};
struct Light {
	vec3 color;
	vec3 direction;
};
#pragma endregion
#pragma region Geometry Functions
Geometry MakeGeometry(const Vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount);//GLsizei = GL Size Index
//Optional, reworded
//Geometry MakeGeometry(const vector<Vertex> verts, const vector<GLuint> indices);

void FreeGeometry(Geometry& geo);

void DrawGeometry(const Shader& shader, const Geometry& geo);
#pragma endregion
#pragma region Shader Functions
Shader MakeShader(const char* vertShader, const char* fragShader);
Shader MakeShader(const string& vertShader, const string& fragShader);
Shader LoadShader(const char* vertPath, const char* fragPath);
void FreeShader(Shader& shader);
#pragma endregion

#pragma region Uniform Functions
void SetUniform(const Shader& shader, GLuint lcoation, const mat4& value);//For t = {}ransforms
void SetUniform(const Shader& shader, GLuint lcoation, const Texture& value, int textureSlot);
void SetUniform(const Shader& shader, GLuint lcoation, const vec4& value);//For color and quaternions
void SetUniform(const Shader& shader, GLuint lcoation, const vec3& value);//For rgb color and positions
#pragma endregion

#pragma region Texture Functions
Texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);
void FreeTexture(Texture& tex);
Texture LoadTexture(const char* imagePath);
#pragma endregion