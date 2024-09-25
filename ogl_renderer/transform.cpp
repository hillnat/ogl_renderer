#include "transform.h"
#include <iostream>
/*
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>//For glm::decompose
*/

void Transform::Translate(const vec3 axis)
{
	matrix = glm::translate(matrix, axis);
}


void Transform::Rotate(const vec3 axis, const float angleDegrees) {
	matrix = glm::rotate(matrix, angleDegrees, axis);
}

 