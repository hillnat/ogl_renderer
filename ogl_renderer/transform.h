#pragma once
#include "glm/ext.hpp"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::quat;
using glm::identity;

class Transform
{	
public:
	Transform(vec3 pos, quat rot, vec3 scale, Transform* parent) {
		this->position = pos;
		this->rotation = rot;
		this->scale = scale;
		this->parent = parent;
	}
	Transform(Transform* parent) {
		this->position = vec3(0.f, 0.f, 0.f);
		this->rotation = identity<quat>();
		this->scale = vec3(1.f, 1.f, 1.f);
		this->parent = parent;
	}
	Transform() {
		this->position = vec3(0.f, 0.f, 0.f);
		this->rotation = identity<quat>();
		this->scale = vec3(1.f, 1.f, 1.f);
		this->parent = nullptr;
	}
	mat4 GetMatrix();
	vec3 position = vec3(0.f, 0.f, 0.f);
	quat rotation = identity<quat>();
	vec3 scale=vec3(1.f,1.f,1.f);
	vec3 foward = rotation * vec3{ 0.f,0.f,1.f };
	vec3 right = vec3(vec4{ 1.f,0.f,0.f,1.f } * rotation);
	vec3 up = rotation * vec3{ 0.f,1.f,0.f };
	void GlobalTranslate(vec3);
	void LocalTranslate(vec3);
	void LocalParentTranslate(vec3);
	void RotateEuler(vec3);
	void RotateAround(vec3,float);
	Transform* parent=nullptr;
};

