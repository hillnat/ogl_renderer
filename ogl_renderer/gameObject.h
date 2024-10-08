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
		this->transform = Transform();
	}
	GameObject(const char* name, const Mesh mesh) //For creating primitives
	{
		this->name = name;
		this->mesh = mesh;
		this->transform = Transform();
	}
	const char* name;
	Transform transform = Transform();
	Mesh mesh;
};

