#include <fstream> //For file reading
#include <iostream> //For file reading
#include "glm/ext.hpp"
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" //For image loading
using std::fstream;

class Scene;
class GameObject;
class Camera;




void Renderer::FreeMesh(Mesh* mesh) {
	if (!mesh) return;
	glDeleteBuffers(1, &mesh->ibo);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteVertexArrays(1, &mesh->vao);
	delete mesh;
}


void Renderer::DrawObjects(Scene* scene) {
	if (scene->gameObjects.empty()) return;
	// Collect unique materials
	std::vector<Material*> uniqueMaterials;
	for (auto& go : scene->gameObjects) {
		if (!go || !go->material) continue;
		if (std::find(uniqueMaterials.begin(), uniqueMaterials.end(), go->material) == uniqueMaterials.end())
			uniqueMaterials.push_back(go->material);
	}

	// Collect unique shaders from materials
	std::vector<Shader*> uniqueShaders;
	for (auto& mat : uniqueMaterials) {
		if (!mat || !mat->shader) continue;
		if (std::find(uniqueShaders.begin(), uniqueShaders.end(), mat->shader) == uniqueShaders.end())
			uniqueShaders.push_back(mat->shader);
	}

	// Write scene-wide uniforms to all shaders
	for (auto& shader : uniqueShaders) {
		if (!shader) continue;

		// Camera
		if (!scene->cameras.empty() && scene->cameras[0]) {
			Camera& mainCamera = *scene->cameras[0];
			Renderer::SetUniform(shader, 1, mainCamera.viewMatrix());
			Renderer::SetUniform(shader, 0, mainCamera.projectionMatrix);
			Renderer::SetUniform(shader, 7, vec3(mainCamera.cameraTransform.GetGlobalPosition()));
		}

		// Lights
		if (!scene->lights.empty() && scene->lights[0]) {
			Renderer::SetUniform(shader, 4, vec3(0.3f));                 // Ambient
			Renderer::SetUniform(shader, 5, scene->lights[0]->color);           // Light color
			Renderer::SetUniform(shader, 6, scene->lights[0]->direction);       // Light direction
		}
	}

	// Draw all game objects
	for (auto& go : scene->gameObjects) {
		if (!go || !go->mesh || !go->material || !go->material->shader) continue;
		if (go->mesh->indexCount == 0) continue;

		Shader* shader = go->material->shader;

		for (int i = 0; i < go->material->textures.size(); i++) {
			Renderer::SetUniform(shader, (GLuint)3, go->material->texArr, (int)i); // Use texture handle and slot i
		}

		// Clear any existing GL errors
		while (glGetError() != GL_NO_ERROR);

		// Handle animations
		if (go->mesh->hasAnimations()) {


			// Check shader program validity
			GLint isProgram = glIsProgram(shader->program);


			// Bind the shader
			glUseProgram(shader->program);


			// Check if bones uniform exists
			GLint bonesLocation = glGetUniformLocation(shader->program, "bones");



			// Update animation
			std::vector<glm::mat4> boneTransforms;
			MeshImporter::UpdateAnimation(go->mesh, (double)glfwGetTime(), boneTransforms);


			if (!boneTransforms.empty()) {
				int boneCount = std::min((int)boneTransforms.size(), 1000);


				// Upload using the correct location
				if (bonesLocation != -1) {
					// Use the actual location from glGetUniformLocation
					glUniformMatrix4fv(bonesLocation, boneCount, GL_FALSE, glm::value_ptr(boneTransforms[0]));


				}

			}


		}
		else if (go->mesh->hasSkinning()) {

			glUseProgram(shader->program);
			GLint bonesLocation = glGetUniformLocation(shader->program, "bones");

			if (bonesLocation != -1) {
				std::vector<glm::mat4> identityBones(go->mesh->bones.size(), glm::mat4(1.0f));
				int boneCount = std::min((int)identityBones.size(), 100);
				glUniformMatrix4fv(bonesLocation, boneCount, GL_FALSE, glm::value_ptr(identityBones[0]));
			}
		}

		// Set model transform
		Renderer::SetUniform(shader, 2, go->transform->matrix);

		// Draw
		Renderer::DrawMesh(shader, go->mesh);

		// Check for errors after drawing
		GLenum finalErr = glGetError();
		if (finalErr != GL_NO_ERROR) {
			std::cout << "ERROR after DrawMesh: " << finalErr << "\n";
		}
	}
}


void Renderer::DrawMesh(const Shader* shader, const Mesh* mesh) {
    if (!shader || !mesh) {
        std::cout << "DrawMesh: NULL shader or mesh\n";
        return;
    }
    
    
    
    // Clear errors
    while (glGetError() != GL_NO_ERROR);
    
    glUseProgram(shader->program);
    GLenum err1 = glGetError();
    if (err1 != GL_NO_ERROR) {
        std::cout << "  ERROR after glUseProgram: " << err1 << "\n";
    }
    
    glBindVertexArray(mesh->vao);
    GLenum err2 = glGetError();
    if (err2 != GL_NO_ERROR) {
        std::cout << "  ERROR after glBindVertexArray: " << err2 << "\n";
    }
    
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);
    GLenum err3 = glGetError();
    if (err3 != GL_NO_ERROR) {
        std::cout << "  ERROR after glDrawElements: " << err3 << "\n";
    }
}

void Renderer::ReportShaderCompileStatus(GLuint& shaderToReport) {
	GLint compileStatus = 0;
	glGetShaderiv(shaderToReport, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		//TODO: Get error report and print it
	}
}

Shader* Renderer::MakeShader(const char* vertshader, const char* fragshader) {
	//TODO: Add error handling/checking logic to shader process
	//Make instance of the shader
	Shader* shader = new Shader();
	shader->program = glCreateProgram();
	
	GLuint vertexPortion = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragPortion = glCreateShader(GL_FRAGMENT_SHADER);
	//Load the shader source code onto the GPU
	glShaderSource(vertexPortion, 1, &vertshader, 0);
	glShaderSource(fragPortion, 1, &fragshader, 0);
	//Compile shaders
	glCompileShader(vertexPortion);
	glCompileShader(fragPortion);
	//Asseble the shader from the shaders
	glAttachShader(shader->program, vertexPortion);
	glAttachShader(shader->program, fragPortion);
	//Combine the shaders we attached and compiled into this final nice thingy
	glLinkProgram(shader->program);

	glDeleteShader(vertexPortion);//Delete the data we used to make our copy
	glDeleteShader(fragPortion);


	// Check shader link status
	GLint linkStatus = 0;
	glGetProgramiv(shader->program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::cout << "ERROR: Shader failed to link!\n";

		// Get error log
		GLint logLength = 0;
		glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			char* log = new char[logLength];
			glGetProgramInfoLog(shader->program, logLength, nullptr, log);
			std::cout << "Shader link error: " << log << "\n";
			delete[] log;
		}
	}
	else {
		std::cout << "Shader linked successfully\n";
	}


	return shader;
}
Shader* Renderer::MakeShader(const string& vertshader, const string& fragshader) {//Overload just coverts params
	return MakeShader(vertshader.c_str(), fragshader.c_str());
}
Shader* Renderer::LoadShader(const char* vertPath, const char* fragPath) {
	string vertSource = ReadFile(vertPath);
	string fragSource = ReadFile(fragPath);



	return MakeShader(vertSource, fragSource);
}
void Renderer::FreeShader(Shader* shader) {
	glDeleteProgram(shader->program);
	delete shader;//Zero out shader struct
}

void Renderer::SetUniform(const Shader* shader, GLuint location, const mat4* value) {
	glProgramUniformMatrix4fv(shader->program, location,1,GL_FALSE, glm::value_ptr(*value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const Texture* value, int textureSlot) {
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, value->handle);
	glProgramUniform1i(shader->program, location, textureSlot);
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const GLuint textureArrayHandle, int textureSlot) {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayHandle);
    glUniform1i(location, textureSlot); // Changed: use glUniform1i instead
}

void Renderer::SetUniform(const Shader* shader, GLuint location, const vec4* value) {
	glProgramUniform4fv(shader->program, location, 1, glm::value_ptr(*value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const vec3* value) {
	glProgramUniform3fv(shader->program, location, 1, glm::value_ptr(*value));
}

void Renderer::SetUniform(const Shader* shader, GLuint location, int count,const vec3* value) {
	glProgramUniform3fv(shader->program, location, ((GLsizei)count), glm::value_ptr(*value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const mat4 value) {
	glProgramUniformMatrix4fv(shader->program, location, 1, GL_FALSE, glm::value_ptr(value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const vec4 value) {
	glProgramUniform4fv(shader->program, location, 1, glm::value_ptr(value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, const vec3 value) {
	glProgramUniform3fv(shader->program, location, 1, glm::value_ptr(value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, int count, const vec3 value) {
	glProgramUniform3fv(shader->program, location, ((GLsizei)count), glm::value_ptr(value));
}
void Renderer::SetUniform(const Shader* shader, GLuint location, int count, const mat4* values) {
	if (!shader || count <= 0 || !values) {
		return;
	}

	glUseProgram(shader->program);  // MUST bind program first
	glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values[0]));
}



string Renderer::ReadFile(const char* path) {//Allows us to load our shaders from files, poorly.
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