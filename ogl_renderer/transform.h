#pragma once
#include "glm/ext.hpp"
#include "physics/AABB.h"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::quat;
using glm::identity;
#include <iostream>
class Transform
{
public:
#pragma region Constructors
	  
	Transform() {
		this->matrix = identity<mat4>();
		//this->parent = nullptr;
	}
#pragma endregion
	//Transform* parent = nullptr;
	mat4 matrix = identity<mat4>();
#pragma region Axis
	vec3 Right() { return vec3(matrix[0]); }
	vec3 Up() { return vec3(matrix[1]); }
	vec3 Forward() { return vec3(matrix[2]); }
#pragma endregion

#pragma region Translation and Rotation
	void TranslateLocal(vec3);
	void TranslateGlobal(vec3);
	void Rotate(vec3,float);
	void SetRotation(vec3,float);
	vec3 GetPosition();
	void ChangeScale(vec3 scalar);
	void LookAt(vec3 pos);
#pragma endregion
	void LogTransform();
};
