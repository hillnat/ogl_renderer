#pragma once
#include "transform.h"
class Camera
{
public:
	Camera() {
		transform = Transform();
	}
	Transform transform;
	const mat4 ProjectionMatrix = glm::perspective(glm::radians(60.f), 1600.f / 900.f, 0.01f, 50000.f);
};