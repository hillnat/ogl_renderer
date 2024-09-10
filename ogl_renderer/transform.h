#pragma once
#include "glm/ext.hpp"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::identity;

class Transform
{	
public:
	Transform(vec3 pos, quat rot, vec3 scale, Transform* parent) {
		position = vec3(0.f, 0.f, 0.f);
		rotation = identity<quat>();
		scale = vec3(1.f, 1.f, 1.f);
	}
	mat4 GetMatrix();
	vec3 position = vec3(0.f, 0.f, 0.f);
	quat rotation = identity<quat>();
	vec3 scale=vec3(1.f,1.f,1.f);
	void GlobalTranslate(vec3);
	void LocalTranslate(vec3);
	void RotateEuler(vec3);
	void RotateQuat(quat);
	Transform* parent=nullptr;
};

