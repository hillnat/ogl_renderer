#pragma once
#include "../engine/Transform.h"
class Camera
{
public:
	Camera() {
		this->cameraTransform = Transform();
	}
	Transform cameraTransform;
	const mat4 projectionMatrix = glm::perspective(glm::radians(90.f), 1600.f / 900.f, 0.01f, 50000.f);
	/// <summary>
	/// Uses glm::lookAt to build a view matrix from cameraTransform
	/// </summary>
	/// <returns> The view matrix of this camera</returns>
	mat4 viewMatrix() {
		return glm::lookAt(cameraTransform.GetGlobalPosition(), cameraTransform.GetGlobalPosition() + cameraTransform.Forward(), cameraTransform.Up());//Generate rotation matrix representing looking forward
	}
};