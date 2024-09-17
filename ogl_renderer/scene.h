#pragma once
#include <vector>
#include "gameObject.h"
#include "camera.h"
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
	void DrawAll(Shader*);
	void SetAllToTestPosition();
	std::vector<GameObject*> gameObjects;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
};

