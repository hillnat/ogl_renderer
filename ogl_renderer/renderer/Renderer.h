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
struct Mesh {
	GLuint vao, vbo, ibo;//vertex Array Object (This is made of the VBO and IBO), vertex Buffer Object (All the vertices), Index Buffer Object (Index buffer holds the orders for vertices to be turned into tris)
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
	Light(vec3 color, vec3 direction) {
		this->color = color;
		this->direction = direction;
	}
	vec3 color = vec3(1.f, 1.f, 1.f);
	vec3 direction = vec3(1.f, 1.f, 1.f);
};
#pragma endregion
#pragma region Meshes
/// <summary>
/// Builds a mesh
/// </summary>
/// <param name="verts">Vertex array</param>
/// <param name="vertCount">Vertex count</param>
/// <param name="indices">Index Array</param>
/// <param name="indexCount">Index Count</param>
/// <returns>The mesh built</returns>
Mesh* MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, const GLsizei indexCount);//GLsizei = GL Size Index
/// <summary>
/// Builds a mesh
/// </summary>
/// <param name="verts">Vertex Array</param>
/// <param name="indices">Index Array</param>
/// <returns>The mesh built</returns>
Mesh* MakeMesh(const vector<vertex> verts, const vector<GLuint> indices);
//Geometry MakeGeometry(const vector<vertex> verts, const vector<GLuint> indices);
/// <summary>
/// Free a mesh from memory
/// </summary>
/// <param name="mesh">The mesh to free</param>
void FreeMesh(Mesh* geo);
/// <summary>
/// Render a given mesh
/// </summary>
/// <param name="shader">Shader program to use</param>
/// <param name="mesh">The mesh to render</param>
void DrawMesh(const Shader* shader, const Mesh* mesh);
#pragma endregion
#pragma region Shader Functions
/// <summary>
/// Builds a shader
/// </summary>
/// <param name="vertShader">The vertex shader to use</param>
/// <param name="fragShader">The fragment shader to use</param>
/// <returns>The shader built</returns>
Shader* MakeShader(const char* vertShader, const char* fragShader);
/// <summary>
/// Builds a shader
/// </summary>
/// <param name="vertShader">The vertex shader to use</param>
/// <param name="fragShader">The fragment shader to use</param>
/// <returns>The shader built</returns>
Shader* MakeShader(const string& vertShader, const string& fragShader);
/// <summary>
/// Load a shader from file
/// </summary>
/// <param name="vertPath">The path of the vertex shader to load</param>
/// <param name="fragPath">The path of the fragment shader to load</param>
/// <returns>The loaded shader</returns>
Shader* LoadShader(const char* vertPath, const char* fragPath);
/// <summary>
/// Frees a shader program from memory
/// </summary>
/// <param name="shader">The shader to free</param>
void FreeShader(Shader& shader);
#pragma endregion
#pragma region Uniform Functions
/// <summary>
/// Sets a shader program uniform with parameters for transforms
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::mat4 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const mat4* value);
/// <summary>
/// Sets a shader program uniform with parameters for textures
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">Texture to use</param>
/// <param name="textureSlot">The texture slot to set</param>
void SetUniform(const Shader* shader, GLuint location, const Texture* value, int textureSlot);
/// <summary>
/// Sets a shader program uniform with parameters for color and quaternions
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::vec4 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const vec4* value);
/// <summary>
/// Sets a shader program uniform with parameters for rgb color and positions
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::vec3 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const vec3* value);
/// <summary>
/// Sets a shader program uniform, with parameters for rgb color and positions arrays
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="count">Count of values in values array</param>
/// <param name="values">glm::vec3 array values to use</param>
void SetUniform(const Shader* shader, GLuint location, int count, const vec3* values);

/// <summary>
/// Sets a shader program uniform with parameters for transforms
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::mat4 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const mat4 value);
/// <summary>
/// Sets a shader program uniform with parameters for color and quaternions
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::vec4 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const vec4 value);
/// <summary>
/// Sets a shader program uniform with parameters for rgb color and positions
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="value">glm::vec3 value to use</param>
void SetUniform(const Shader* shader, GLuint location, const vec3 value);
/// <summary>
/// Sets a shader program uniform, with parameters for rgb color and positions arrays
/// </summary>
/// <param name="shader">Shader to use</param>
/// <param name="location">Uniform location in shader</param>
/// <param name="count">Count of values in values array</param>
/// <param name="values">glm::vec3 array values to use</param>
void SetUniform(const Shader* shader, GLuint location, int count, const vec3* values);
#pragma endregion
#pragma region Texture Functions
/// <summary>
/// Builds a texture
/// </summary>
/// <param name="width">Width of the texture in pixels</param>
/// <param name="height">Height of the texture in pixels</param>
/// <param name="channels">The channels</param>
/// <param name="pixels">The pixels</param>
/// <returns></returns>
Texture* MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels);
/// <summary>
/// Free a texture from memory
/// </summary>
/// <param name="tex">The texture to free</param>
void FreeTexture(Texture* tex);
/// <summary>
/// Loads a texture from file
/// </summary>
/// <param name="imagePath">The path of the texture</param>
/// <returns>The loaded texture</returns>
Texture* LoadTexture(const char* imagePath);
#pragma endregion
/// <summary>
/// Read from a file
/// </summary>
/// <param name="path">File path to use</param>
/// <returns>File contents</returns>
string ReadFile(const char* path);
