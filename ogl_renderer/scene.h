#pragma once
#include <vector>
#include "gameObject.h"
#include "camera.h"
class scene
{
public:
	scene() {
		gameObjects.clear();
		cameras.clear();
		lights.clear();
	}
	void AddToScene(gameObject*);
	void AddToScene(camera*);
	void AddToScene(light*);
	void RenderAll(shader*);
	std::vector<gameObject*> gameObjects;
	std::vector<camera*> cameras;
	std::vector<light*> lights;
};

