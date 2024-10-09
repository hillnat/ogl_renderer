#include "scene.h"
#include "glm/ext.hpp"
#include "renderer.h"
void scene::AddToScene(gameObject* go) {
	gameObjects.push_back(go);
}
void scene::AddToScene(camera* cam) {
	cameras.push_back(cam);
}
void scene::AddToScene(light* light) {
	lights.push_back(light);
}

void scene::RenderAll(shader* shader) {
	//Handle Camera
	if (cameras.size() != 0 && cameras[0] != nullptr) {
		camera mainCamera = *cameras[0];
		SetUniform(*shader, 1, mainCamera.viewMatrix());
		SetUniform(*shader, 0, mainCamera.ProjectionMatrix);
		SetUniform(*shader, 7, vec3(mainCamera.transform.GetPosition()));//Cam position for specular. Mat4 to vec3 conversion
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