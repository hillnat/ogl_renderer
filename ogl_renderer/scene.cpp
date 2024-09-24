#include "scene.h"
#include "glm/ext.hpp"
void Scene::AddToScene(GameObject* go) {
	gameObjects.push_back(go);
}
void Scene::AddToScene(Camera* cam) {
	cameras.push_back(cam);
}
void Scene::AddToScene(Light* light) {
	lights.push_back(light);
}
void Scene::DrawAll(Shader* shader) {
	//Handle Camera
	if (cameras.size() != 0 && cameras[0] != nullptr) {
		Camera mainCamera = *cameras[0];
		SetUniform(*shader, 1, mainCamera.transform.matrix);//Camera transform
		SetUniform(*shader, 0, mainCamera.ProjectionMatrix);
		SetUniform(*shader, 7, vec3(mainCamera.transform.matrix[3]));//Cam position for specular. Mat4 to vec3 conversion
	}
	//Handle all lights
	for (int i = 0; i < lights.size(); i++) {
		SetUniform(*shader, 5, (*lights[i]).color);
		SetUniform(*shader, 6, (*lights[i]).direction);
	}
	//Handle all objects
	for (int i = 0; i < gameObjects.size(); i++) {
		SetUniform(*shader, 2, (*gameObjects[i]).transform.matrix);
		if (gameObjects[i]->mesh.size !=0) {
			DrawMesh(*shader, (*gameObjects[i]).mesh);
		}		
	}	
}