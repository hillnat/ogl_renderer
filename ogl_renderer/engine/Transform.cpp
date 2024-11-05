#include "Transform.h"
#include <iostream>
/*
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>//For glm::decompose
*/

void Transform::TranslateLocal(const vec3 axis)
{
	matrix = glm::translate(matrix, axis);
	//LogTransform();
}
void Transform::TranslateGlobal(const vec3 axis)
{
	matrix[3].x += axis.x;
	matrix[3].y += axis.y;
	matrix[3].z += axis.z;

	//LogTransform();
}
void Transform::ChangeScale(const vec3 scalar) {
	matrix = glm::scale(matrix, scalar);
	//LogTransform();
}
void Transform::Rotate(const vec3 axis, const float angleDegrees) {
	matrix = glm::rotate(matrix, glm::radians(angleDegrees), glm::normalize(axis));
	//LogTransform();
}
void Transform::SetRotation(const vec3 axis, const float angleDegrees) {
	matrix = glm::rotate(glm::identity<mat4>(), glm::radians(angleDegrees), glm::normalize(axis));
	//LogTransform();
}
vec3 Transform::GetPosition() {
	vec3 pos = vec3(matrix[3]);
	if (parent != nullptr) {pos += GetAccumParentPosition(this); }
	return pos;
}
void Transform::LookAt(vec3 pos) {
	matrix = glm::lookAt(GetPosition(), pos, vec3(0, 1, 0));
	//Rotate(vec3(0, 1, 0), 180);
}
void Transform::LogTransform() {
	std::cout << "Forward : " << Forward().x << " | " << Forward().y << " | " << Forward().z << " | " << std::endl;
	std::cout << "Up      : " << Up().x      << " | " << Up().y      << " | " << Up().z      << " | " << std::endl;
	std::cout << "Right   : " << Right().x   << " | " << Right().y   << " | " << Right().z   << " | " << std::endl;
	std::cout << "-----------------------------------------------------------------" << std::endl;
}
vec3 Transform::GetAccumParentPosition(Transform* transform) {
	if (transform->parent != nullptr) {
		return GetAccumParentPosition(transform->parent);
	} else {
		return vec3(0, 0, 0);
	}
}

 