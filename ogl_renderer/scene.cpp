#include "scene.h"
#include "glm/ext.hpp"
#include "renderer.h"
void Scene::AddToScene(GameObject* go) {
	gameObjects.push_back(go);
}
void Scene::AddToScene(Camera* cam) {
	cameras.push_back(cam);
}
void Scene::AddToScene(Light* light) {
	lights.push_back(light);
}

void Scene::DrawAll(Shader* shader, float deltaTime) {
	//Handle Camera
	if (cameras.size() != 0 && cameras[0] != nullptr) {
		Camera mainCamera = *cameras[0];
		SetUniform(*shader, 1, mainCamera.transform.matrix);//Camera transform
		SetUniform(*shader, 0, mainCamera.ProjectionMatrix);
		SetUniform(*shader, 7, vec3(mainCamera.transform.matrix[3]));//Cam position for specular. Mat4 to vec3 conversion
	}
	SetUniform(*shader, 4, vec3(0, 0, 0) );//Set ambient
	SetUniform(*shader, 5, lights[0]->color);
	SetUniform(*shader, 6, lights[0]->direction);
	//Handle all objects
	for (int i = 0; i < gameObjects.size(); i++) {
		SetUniform(*shader, 2, (*gameObjects[i]).transform.matrix);
		if (gameObjects[i]->mesh.size !=0) {
			DrawMesh(*shader, (*gameObjects[i]).mesh);
		}		
	}	
}