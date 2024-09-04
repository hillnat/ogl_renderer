#pragma once
#include "transform.h"
class Camera
{
public:
	Transform transform;
	mat4 ProjectionMatrix = glm::perspective(glm::radians(70.f), 640.f / 480.f, 0.01f, 50000.f);
};

