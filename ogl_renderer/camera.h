#pragma once
#include "transform.h"
class camera
{
public:
	camera() {
		transform = Transform();
	}
	Transform transform;
	const mat4 projectionMatrix = glm::perspective(glm::radians(90.f), 1600.f / 900.f, 0.01f, 50000.f);
	mat4 viewMatrix() {
		return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.Forward(), transform.Up());//Generate rotation matrix representing looking forward
	}
};