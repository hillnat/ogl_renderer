#pragma once
#include "transform.h"
class Camera
{
public:
	Camera(vec3 pos, quat rot, vec3 scale, Transform* parent) {
		transform = Transform(pos, rot, scale, parent);
	}
	Transform transform;
	mat4 ProjectionMatrix = glm::perspective(glm::radians(70.f), 640.f / 480.f, 0.01f, 50000.f);
};

