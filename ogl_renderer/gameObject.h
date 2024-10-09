#pragma once
#include "transform.h"
#include "renderer.h"
#include "meshImporter.h"
class gameObject
{
public:
	gameObject(const char* name, const char* meshPath) {
		this->name = name;
		this->mesh = MeshImporter::ImportFromFile(meshPath);
		this->transform = Transform();
	}
	gameObject(const char* name, const mesh mesh) //For creating primitives
	{
		this->name = name;
		this->mesh = mesh;
		this->transform = Transform();
	}
	const char* name;
	Transform transform = Transform();
	mesh mesh;
};

