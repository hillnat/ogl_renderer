#include "transform.h"
/*
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>//For glm::decompose
*/

void Transform::GlobalTranslate(vec3 pos)//Global position
{
	position += pos;
}
void Transform::LocalTranslate(vec3 pos)//Relative to rotation
{
	position += glm::normalize(rotation)*pos;
}
void Transform::LocalParentTranslate(vec3 pos)//Relative to parent rotation
{
	if (parent == nullptr) { return; }
	position += glm::normalize(parent->rotation) * pos;
}
void Transform::RotateEuler(vec3 eulerAngles) {
	quat newRotationQuat = glm::quat(glm::radians(eulerAngles));
	quat finalQuat;
	if (parent != nullptr) { finalQuat = parent->rotation*rotation; }
	else { finalQuat = rotation; }
	finalQuat = finalQuat * newRotationQuat;
	rotation = finalQuat;
}
void Transform::RotateEulerClamped(vec3 eulerAngles, vec2 clamp) {
	quat newRotationQuat = glm::quat(glm::radians(eulerAngles));
	quat finalQuat = rotation * newRotationQuat;
	rotation = finalQuat;
}
mat4 Transform::GetMatrix() {//Construct matrix and account for parent
	quat rotationQuat=rotation;
	vec3 scaleVec = scale;
	vec3 posVec = position;
	if (parent != nullptr) { 
		scaleVec *= parent->scale;
		rotationQuat *= parent->rotation;
		posVec += parent->position;
	}
	mat4 rotationMatrix = mat4_cast(rotationQuat);
	mat4 scaleMatrix = glm::scale(mat4(1.0f), scaleVec);
	mat4 translationMatrix = translate(mat4(1.0f), posVec);

	mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return transformMatrix;
}
 