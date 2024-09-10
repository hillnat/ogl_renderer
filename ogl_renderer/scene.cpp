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
void Scene::DrawAll(Shader* basicShader) {
	if (cameras.size() != 0 && cameras[0] != nullptr) {
		Camera mainCamera = *cameras[0];
		SetUniform(*basicShader, 1, glm::inverse(mainCamera.transform.GetMatrix()));//Camera transform
		SetUniform(*basicShader, 7, vec3(mainCamera.transform.position));//Cam position for specular. Mat4 to vec3 conversion
	}
	for (int i = 0; i < gameObjects.size(); i++) {
		SetUniform(*basicShader, 2, (*gameObjects[i]).transform.GetMatrix());//Cube transform
		DrawMesh(*basicShader, (*gameObjects[i]).mesh);
	}	
}