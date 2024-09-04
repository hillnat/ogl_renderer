#pragma once
#include "glm/ext.hpp"
using glm::mat4;
using glm::vec3;

class Transform
{	
public:
	Transform() {
		matrix = glm::identity<mat4>();
	}
	mat4 matrix;
	void Translate(vec3 dir);
	void TranslateL(vec3 dir);
	void Rotate(float angle, vec3 dir);
	void LookAt(vec3 lookAt, vec3 axis);
};

