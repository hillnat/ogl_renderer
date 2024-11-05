#include "scene.h"
#include "glm/ext.hpp"
#include "../renderer/renderer.h"

void Scene::AddToScene(GameObject* go) {
	gameObjects.push_back(go);
}
void Scene::AddToScene(Camera* cam) {
	cameras.push_back(cam);
}
void Scene::AddToScene(Light* light) {
	lights.push_back(light);
}
void Scene::FreeAllMeshes() {
	for (int i = 0; i < gameObjects.size(); i++) {
		FreeMesh(gameObjects[i]->mesh);
	}
}
void Scene::RenderAll() {
	std::vector<Material*> uniqueMaterials;
	//Get all unique materials gameobjects
	for (int i = 0; i < gameObjects.size(); i++) {
		if (uniqueMaterials.size() != 0) {
			for (int j = 0; j < uniqueMaterials.size(); j++) {
				if (gameObjects[i]->material != uniqueMaterials[j]) { uniqueMaterials.push_back((*gameObjects[i]).material); }
			}
		}
		else{ uniqueMaterials.push_back((*gameObjects[i]).material); }
	}
	std::vector<Shader*> uniqueShaders;
	//Get all unique shaders from materials
	for (int i = 0; i < uniqueMaterials.size(); i++) {
		if (uniqueShaders.size() != 0) {
			for (int j = 0; j < uniqueShaders.size(); j++) {
				if ((*uniqueMaterials[i]).shader != uniqueShaders[j]) { uniqueShaders.push_back((*uniqueMaterials[i]).shader); }
			}
		}
		else { uniqueShaders.push_back((*uniqueMaterials[i]).shader); }
	}
	//Write uniforms to all shaders
	for (int i = 0; i < uniqueShaders.size(); i++) {
		Shader* shader = uniqueShaders[i];
		//Handle Camera
		if (cameras.size() != 0 && cameras[0] != nullptr) {
			Camera mainCamera = *cameras[0];
			SetUniform(*shader, 1, mainCamera.viewMatrix());
			SetUniform(*shader, 0, mainCamera.projectionMatrix);
			SetUniform(*shader, 7, vec3(mainCamera.cameraTransform.GetPosition()));//Cam position for specular. Mat4 to vec3 conversion
		}
		SetUniform(*shader, 4, vec3(0.1f, 0.1f, 0.1f));//Set ambient
		SetUniform(*shader, 5, lights[0]->color);
		SetUniform(*shader, 6, lights[0]->direction);
	}
	
	//Draw all objects, setting object based uniforms for their respective shader
	for (int i = 0; i < gameObjects.size(); i++) {
		SetUniform(*(*gameObjects[i]).material->shader, 2, (*gameObjects[i]).transform.matrix);
		if (gameObjects[i]->mesh.size !=0) {
			DrawMesh(*(*gameObjects[i]).material->shader, (*gameObjects[i]).mesh);
		}		
	}	
}