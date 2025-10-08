#pragma once
#include <vector>
#include "GameObject.h"

#include "../renderer/Camera.h"
#include "Primitives.h"
class Scene
{
private:
	
public:
	Scene() {
		gameObjects.clear();
		cameras.clear();
		lights.clear();
	}

	std::vector<GameObject*> gameObjects;

	std::vector<Camera*> cameras;

	std::vector<Light*> lights;

	void AddToScene(GameObject* gameObject);

	void AddToScene(Camera* camera);

	void AddToScene(Light* light);

	void FreeAllMeshes();
};

