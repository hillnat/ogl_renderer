#pragma once
#include "transform.h"
class Camera
{
public:
	Camera() {
		this->cameraTransform = Transform();
	}
	Transform cameraTransform;
	const mat4 projectionMatrix = glm::perspective(glm::radians(90.f), 1600.f / 900.f, 0.01f, 50000.f);
	mat4 viewMatrix() {
		return glm::lookAt(cameraTransform.GetPosition(), cameraTransform.GetPosition() + cameraTransform.Forward(), cameraTransform.Up());//Generate rotation matrix representing looking forward
	}
};