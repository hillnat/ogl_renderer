#pragma once
#include "glm/ext.hpp"
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
	vec4 Right() {return matrix[0]; }
	vec4 Up() {return matrix[1]; }
	vec4 Forward() {return matrix[2]; }
#pragma endregion

#pragma region Translation and Rotation
	void Translate(vec3);
	void Rotate(vec3,float);
#pragma endregion

};
