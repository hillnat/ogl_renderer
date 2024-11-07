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
		std::cout << "Constructing GameObject" << std::endl;
		this->name = name;
		this->mesh = (MeshImporter::ImportFromFile(meshPath));
		this->transform = new Transform();
		this->material = material;
	}
	GameObject(const char* name, Mesh* mesh, Material* material)
	{
		std::cout << "Constructing GameObject" << std::endl;

		this->name = name;
		this->mesh = mesh;
		this->transform = new Transform();
		this->material = material;
	}
	GameObject(Mesh* mesh, Material* material)
	{
		std::cout << "Constructing GameObject" << std::endl;

		this->name = "GameObject";
		this->mesh = mesh;
		this->transform = new Transform();
		this->material = material;
	}
	GameObject(const char* meshPath, Material* material)
	{
		std::cout << "Constructing GameObject" << std::endl;

		this->name = "GameObject";
		this->mesh = MeshImporter::ImportFromFile(meshPath);
		this->transform = new Transform();
		this->material = material;
	}
	~GameObject() {
		std::cout << name << " is deconstructing" << std::endl;
	}
	const char* name;
	Transform* transform;
	Mesh* mesh;
	Material* material;
};

