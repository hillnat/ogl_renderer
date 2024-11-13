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
void Transform::SetGlobalPosition(const vec3 position) {
	TranslateGlobal(-GetGlobalPosition());//Set to 0,0,0
	TranslateGlobal(position);//Set position
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

vec3 Transform::GetLocalPosition() {
	if (parent != nullptr) { vec3(matrix[3]) + parent->GetGlobalPosition(); }
	else { return vec3(matrix[3]); }
}
vec3 Transform::GetGlobalPosition() {
	return vec3(matrix[3]);
}
void Transform::LookAt(vec3 pos) {
	matrix = glm::lookAt(GetGlobalPosition(), pos, vec3(0, 1, 0));
	//Rotate(vec3(0, 1, 0), 180);
}
void Transform::LogTransform() {
	vec3 gpos = GetGlobalPosition();
	vec3 lpos = GetLocalPosition();
	std::cout << "Global Position : " << gpos.x << " | " << gpos.y << " | " << gpos.z << " | " << std::endl;
	std::cout << "Local Position : " << lpos.x << " | " << lpos.y << " | " << lpos.z << " | " << std::endl;
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

 