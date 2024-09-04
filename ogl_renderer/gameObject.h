#pragma once
#include "transform.h"
#include "renderer.h"
#include "meshImporter.h"
class GameObject
{
public:
	GameObject(const char* name, const char* meshPath) {
		this->name = name;
		this->mesh = MeshImporter::ImportFromFile(meshPath);
	}
	const char* name;
	Transform transform;
	Mesh mesh;
};

