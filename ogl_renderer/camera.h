#pragma once
#include "transform.h"
class Camera
{
public:
	Camera(Transform* parent, vec3 pos, quat rot, vec3 scale) {
		transform = Transform(pos, rot, scale, parent);
	}
	Camera(Transform* parent, vec3 pos) {
		transform = Transform(pos);
	}
	Camera(vec3 pos) {
		transform = Transform(pos);
	}
	Camera(Transform* parent) {
		transform = Transform(parent);
	}
	Camera() {
		transform = Transform();
	}
	Transform transform;
	const mat4 ProjectionMatrix = glm::perspective(glm::radians(90.f), 1600.f / 900.f, 0.01f, 50000.f);
};

