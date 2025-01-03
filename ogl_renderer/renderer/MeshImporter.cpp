#include "MeshImporter.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>
#include "glm/ext.hpp"
#include "Renderer.h"
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>

Mesh* MeshImporter::ImportFromFile(const char* filename)
{
	//std::cout << "Loading mesh from file path : " << filename << std::endl;
	// read vertices from the model
	float startTime = glfwGetTime();
	const aiScene* scene = aiImportFile(filename, 0);
	if (scene == NULL) { std::cout << "Fatal error" << std::endl; return nullptr; }
	std::vector<vertex> allVertices;
	std::vector<GLuint> allIndices;
	int allVertexCount = 0;
	
	for (int m = 0; m < scene->mNumMeshes; m++) {

		aiMesh* mesh = scene->mMeshes[m];
		//extract indicies from the first mesh
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			allIndices.push_back(mesh->mFaces[i].mIndices[1]);

			allIndices.push_back(mesh->mFaces[i].mIndices[2]);
			allIndices.push_back(mesh->mFaces[i].mIndices[0]);
			// generate a second triangle for quads
			if (mesh->mFaces[i].mNumIndices == 4)
			{


				allIndices.push_back(mesh->mFaces[i].mIndices[0]);
				allIndices.push_back(mesh->mFaces[i].mIndices[2]);

				allIndices.push_back(mesh->mFaces[i].mIndices[3]);

				
			}
		}
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			vertex vertex = {};
			vertex.pos = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);

			if (mesh->HasVertexColors(i)) {
				aiColor4D* vertexColor = mesh->mColors[i];
				vertex.color = vec4{ vertexColor->r,  vertexColor->g, vertexColor->b, vertexColor->a };
			}
			vertex.color = vec4{ 1,  1, 1, 1 };

			if (mesh->HasNormals()) {
				vertex.normal = vec3{ mesh->mNormals[i].x,mesh->mNormals[i].y,  mesh->mNormals[i].z };
			}
			if (mesh->HasTextureCoords(0)) {//Note pass in 0 for the check, and grab the mtextureCoords from a 2D array
				vertex.uv = vec2{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			/*std::cout << "vert index : " << i << std::endl;
			std::cout << "color " << vertex.color.x << ' ' << vertex.color.y << ' ' << vertex.color.z << ' ' << std::endl;
			std::cout << "pos " << vertex.pos.x << ' ' << vertex.pos.y << ' ' << vertex.pos.z << ' ' << std::endl;
			std::cout << "normal " << vertex.normal.x << ' ' << vertex.normal.y << ' ' << vertex.normal.z << ' ' << std::endl;
			std::cout << "uv " << vertex.uv.x << ' ' << vertex.uv.y << ' ' << std::endl;
			std::cout << "-------------------------" << std::endl;*/
			allVertices.push_back(vertex);
		}
		allVertexCount += mesh->mNumVertices;//Add num of vertices found in submesh to total vert count
	}
	
	float endTime = glfwGetTime();
	std::cout << "Succeeded loading "<< allVertexCount<< " vertices after " << endTime-startTime << "s elapsed." << std::endl;
	return MakeMesh(allVertices.data(), allVertexCount, allIndices.data(), allIndices.size());
	//delete[] vertices;
	//m_quadMesh.initialiseFromFile("stanford/bunny.obj");
}