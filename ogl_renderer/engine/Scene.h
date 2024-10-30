#pragma once
#include <vector>
#include "GameObject.h"
#include "../renderer/Camera.h"
class Scene
{
public:
	Scene() {
		gameObjects.clear();
		cameras.clear();
		lights.clear();
	}
	void AddToScene(GameObject*);
	void AddToScene(Camera*);
	void AddToScene(Light*);
	void FreeAllMeshes();
	void RenderAll();
	std::vector<GameObject*> gameObjects;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
};

