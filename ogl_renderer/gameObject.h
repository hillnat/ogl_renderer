#pragma once
#include "transform.h"
#include "renderer.h"
#include "meshImporter.h"
class GameObject
{
public:
	GameObject(const char* name, const char* meshPath, vec3 pos, quat rot, vec3 scale, Transform* parent) {
		this->name = name;
		this->mesh = MeshImporter::ImportFromFile(meshPath);
		this->transform = Transform(pos,rot,scale,parent);
	}
	GameObject(const char* name, const char* meshPath) {
		this->name = name;
		this->mesh = MeshImporter::ImportFromFile(meshPath);
		this->transform = Transform();
	}
	const char* name;
	Transform transform;
	Mesh mesh;
};

