#pragma once
#include "Transform.h"
#include "../renderer/Renderer.h"
#include "../renderer/Material.h"
#include "../renderer/MeshImporter.h"
#include "../physics/AABB.h"
class GameObject
{
public:
	GameObject(const char* name, const char* meshPath, Material* material) {
		this->name = name;
		this->mesh = MeshImporter::ImportFromFile(meshPath);
		this->transform = Transform();
		this->material = material;
	}
	GameObject(const char* name, const Mesh mesh, Material* material) //For creating primitives
	{
		this->name = name;
		this->mesh = mesh;
		this->transform = Transform();
		this->material = material;
	}
	const char* name;
	Transform transform = Transform();
	Mesh mesh;
	Material* material;
};

