#include "transform.h"
/*
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>//For glm::decompose
*/

void Transform::GlobalTranslate(vec3 pos) {
	position += pos;
}
void Transform::LocalTranslate(vec3 pos) {
	position += glm::normalize(rotation)*pos;
}

void Transform::RotateEuler(vec3 eulerAngles) {
	quat newRotationQuat = glm::quat(glm::radians(eulerAngles));
	quat finalQuat = rotation * newRotationQuat;
	rotation = finalQuat;
}
void Transform::RotateQuat(quat quatRot) {
	quat newRotationQuat = quatRot;
	quat finalQuat = rotation * newRotationQuat;
	rotation = finalQuat;
}
mat4 Transform::GetMatrix() {
	mat4 translationMatrix = translate(mat4(1.0f), position);
	mat4 rotationMatrix = mat4_cast(rotation);
	mat4 scaleMatrix = glm::scale(mat4(1.0f), scale);
	mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return transformMatrix;
}
 