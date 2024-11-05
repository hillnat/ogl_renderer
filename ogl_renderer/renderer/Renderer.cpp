#include <fstream> //For file reading
#include "glm/ext.hpp"
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" //For image loading
using std::fstream;
#pragma region Meshes
Mesh MakeMesh(const vertex* const verts, GLsizei vertCount, const GLuint* indices, GLsizei indexCount)
{
	Mesh newmesh = Mesh{};//Zero-initialize
	newmesh.size = indexCount;//vertex count
	//Make bufers
	glGenVertexArrays(1, &newmesh.vao);//Make a vertex Array Object at this Geo's point in memory on the GPU
	glGenBuffers(1, &newmesh.vbo);//Generate the buffer of vertices
	glGenBuffers(1, &newmesh.ibo);//Generate the buffer of vertex indecies
	//Note this is generating the buffers "into" the geometry object
	//Bind the buffers for the task which we want them to do, like an angry spirit - Rob
	glBindVertexArray(newmesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, newmesh.vbo);//Bind the buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newmesh.ibo);

	//Use vertex data if using vectors... the GPU Manufacterer determines how to draw it
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

#pragma region Enable vertex Attributes
	//Describe the buffer
	//vertex position, index 0
	glEnableVertexAttribArray(0);//Attribute of index for enabling position
	glVertexAttribPointer(
		0,//Attribute index
		4,//Component count (4 because Vec4 for position)
		GL_FLOAT,//Component type, float
		GL_FALSE,//False for dont normalize. You'd want to normalize if you were treating this as a direction
		sizeof(vertex),//Byte size of each vertex position, need to know because its a stream
		(void*)0//Pass in 0 bytes because we dont need to offset the stream yet.
	);
	//vertex color blending, index 1
	glEnableVertexAttribArray(1);//Attribute of index for enabling vertex color
	glVertexAttribPointer(
		1,//Attribute index
		4,//Component count (4 because Vec4 for color)
		GL_FLOAT,//Component type, also float
		GL_FALSE,//False for dont normalize.
		sizeof(vertex),//Byte size of each vertex position, need to know because its a stream
		(void*)offsetof(vertex, color)//Pass in the bye offset between start of vertex struct and color vec4 within that struct, because we are futher down the stream
	);
	//UV maps, index 2
	glEnableVertexAttribArray(2);//Attribute of index for enabling UV maps to Vertices
	glVertexAttribPointer(
		2,//Attribute index
		2,//Component count (2 because Vec2 for UV)
		GL_FLOAT,//Component type, also float
		GL_FALSE,//False for dont normalize. UV maps are conventially between 0 and 1, however they can be used past those values, so we arent going to normalize
		sizeof(vertex),//Byte size of each vertex position, need to know because its a stream
		(void*)offsetof(vertex, uv)//Pass in the bye offset between vertex and uv, because we are futher down the stream
	);
	//Normal maps, index 3
	glEnableVertexAttribArray(3);//Attribute of index for enabling UV maps to Vertices
	glVertexAttribPointer(
		3,//Attribute index
		3,//Component count (3 because Vec3 for normal)
		GL_FLOAT,//Component type, also float
		GL_FALSE,//False for dont normalize. UV maps are conventially between 0 and 1, however they can be used past those values, so we arent going to normalize
		sizeof(vertex),//Byte size of each vertex position, need to know because its a stream
		(void*)offsetof(vertex, normal)//Pass in the bye offset between vertex and uv, because we are futher down the stream
	);
#pragma endregion
	//Unbind the angry spirit, aka bind it to nothing
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);//Bind the buffers to nothing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return newmesh;	//Fuck yeah
	// |	 |
	// \(o.o)/
	//	  |
	//    |
	//	  ^
	//	 | |
	//  d   b
}
//Overload, if you dare.
Mesh MakeMesh(const vector<vertex> verts, const vector<GLuint> indices) {//Overload just calls the same func above with different data
	return MakeMesh(verts.data(), verts.size(), indices.data(), indices.size());
}
void FreeMesh(Mesh& mesh) {
	//DELETE BUFFERS IN REVERSE ORDER THEY WERE GENERATED
	glDeleteBuffers(1, &mesh.ibo);
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteVertexArrays(1, &mesh.vao);
	//Zero out the geo so its gone from gpu memory
	mesh = {};
}
void DrawMesh(const Shader& shader, const Mesh& mesh) {
	//Specify which shader to use
	glUseProgram(shader.program);
	//Specify which geometry
	glBindVertexArray(mesh.vao);
	glDrawElements(
		GL_TRIANGLES,//GL_LINES
		mesh.size,
		GL_UNSIGNED_INT,
		nullptr
	);
}
#pragma endregion
#pragma region Shader
void ReportCompileStatus(GLuint& shaderToReport) {
	GLint compileStatus = 0;
	glGetShaderiv(shaderToReport, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		//TODO: Get error report and print it
	}
}

Shader MakeShader(const char* vertshader, const char* fragshader) {
	//TODO: Add error handling/checking logic to shader process
	//Make instance of the shader
	Shader shader = {};
	shader.program = glCreateProgram();
	
	GLuint vertexPortion = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragPortion = glCreateShader(GL_FRAGMENT_SHADER);
	//Load the shader source code onto the GPU
	glShaderSource(vertexPortion, 1, &vertshader, 0);
	glShaderSource(fragPortion, 1, &fragshader, 0);
	//Compile shaders
	glCompileShader(vertexPortion);
	glCompileShader(fragPortion);
	//Asseble the shader from the shaders
	glAttachShader(shader.program, vertexPortion);
	glAttachShader(shader.program, fragPortion);
	//Combine the shaders we attached and compiled into this final nice thingy
	glLinkProgram(shader.program);
	//TODO: Error check the link
	glDeleteShader(vertexPortion);//Delete the data we used to make our copy
	glDeleteShader(fragPortion);

	return shader;
}
Shader MakeShader(const string& vertshader, const string& fragshader) {//Overload just coverts params
	return MakeShader(vertshader.c_str(), fragshader.c_str());
}
Shader LoadShader(const char* vertPath, const char* fragPath) {
	string vertSource = ReadFile(vertPath);
	string fragSource = ReadFile(fragPath);
	return MakeShader(vertSource, fragSource);
}
void FreeShader(Shader& shader) {
	glDeleteProgram(shader.program);
	shader = {};//Zero out shader struct
}
#pragma endregion
#pragma region Uniforms
void SetUniform(const Shader& shader, GLuint location, const mat4& value) {
	glProgramUniformMatrix4fv(shader.program, location,1,GL_FALSE, glm::value_ptr(value));
}
void SetUniform(const Shader* shader, GLuint location, const Texture* value, int textureSlot) {
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, value->handle);
	glProgramUniform1i(shader->program, location, textureSlot);
}
void SetUniform(const Shader& shader, GLuint location, const vec4& value) {
	glProgramUniform4fv(shader.program, location, 1, glm::value_ptr(value));
}
void SetUniform(const Shader& shader, GLuint location, const vec3& value) {
	glProgramUniform3fv(shader.program, location, 1, glm::value_ptr(value));
}
void SetUniform(const Shader& shader, GLuint location, int count,const vec3& value) {
	glProgramUniform3fv(shader.program, location, ((GLsizei)count), glm::value_ptr(value));
}
#pragma endregion
#pragma region Texture
Texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels) {
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
	Texture textObj = { 0, width, height, channels };
	glGenTextures(1, &textObj.handle);
	//bind and buffer texture
	glBindTexture(GL_TEXTURE_2D, textObj.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);
	//Configure texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Unbind and return the object
	glBindTexture(GL_TEXTURE_2D, 0);
	return textObj;
}

void FreeTexture(Texture& tex) {
	glDeleteTextures(1, &tex.handle);
	tex = {};
}
Texture LoadTexture(const char* imagePath) {
	//Setup varaibles to store texture data
	int imageWidth = -1;//-1 for error handling
	int imageHeight = -1;
	int imageFormat = -1;
	unsigned char* imagePixels = nullptr; 
	Texture newtexture = {};
	stbi_set_flip_vertically_on_load(true);//Load using OpenGLConventions
	imagePixels = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	//Note : stdi_load will return a nullptr if it failed to load
	//Pass the data to OpenGL
	newtexture = MakeTexture(imageWidth, imageHeight, imageFormat, imagePixels);
	stbi_image_free(imagePixels);
	imagePixels = nullptr;
	return newtexture;
}
#pragma endregion
string ReadFile(const char* path) {//Allows us to load our shaders from files, poorly.
	fstream stream(path, std::ios_base::in);//Open the freakstream
	string source;
	//TODO: Add error checking and validation if you are reading from a text file
	while (!stream.eof()) {
		string thisLine;
		std::getline(stream, thisLine);
		source += thisLine + "\n";
	}
	stream.close();//Close the freakstream
	return source;
}