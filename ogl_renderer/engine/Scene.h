#pragma once
#include <vector>
#include "GameObject.h"
#include "../renderer/Camera.h"
class Scene
{
private:
	
public:
	Scene() {
		gameObjects.clear();
		cameras.clear();
		lights.clear();
	}
	/// <summary>
	/// List of all GameObjects in scene
	/// </summary>
	std::vector<GameObject*> gameObjects;
	/// <summary>
	/// List of all Cameras in scene
	/// </summary>
	std::vector<Camera*> cameras;
	/// <summary>
	/// List of all Lights in scene
	/// </summary>
	std::vector<Light*> lights;
	/// <summary>
	/// Add a gameObject to the scene
	/// </summary>
	/// <param name="gameObject">GameObject to add</param>
	void AddToScene(GameObject* gameObject);
	/// <summary>
	/// Add a camera to the scene
	/// </summary>
	/// <param name="camera">Camera to add</param>
	void AddToScene(Camera* camera);
	/// <summary>
	/// Add a light to the scene
	/// </summary>
	/// <param name="light">Light to add</param>
	void AddToScene(Light* light);
	/// <summary>
	/// Free all the meshes attached to gameObjects in gameObjects
	/// </summary>
	void FreeAllMeshes();
	/// <summary>
	/// Draw all gameObjects using cameras[0] as a camera, accounting for all lights
	/// </summary>
	void RenderAll();
};

