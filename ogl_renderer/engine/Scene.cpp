#include "scene.h"
#include "glm/ext.hpp"
#include "../renderer/renderer.h"
#include <iostream>
#include <algorithm>

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
        Renderer::FreeMesh(gameObjects[i]->mesh);
    }
}

