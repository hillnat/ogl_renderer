#pragma once
#include "transform.h"
class Camera
{
public:
	Camera(Transform* parent, vec3 pos, quat rot, vec3 scale) {
		transform = Transform(pos, rot, scale, parent);
	}
	Camera(Transform* parent, vec3 pos) {
		transform = Transform(pos, glm::identity<quat>(), vec3{1.f,1.f,1.f}, nullptr);
	}
	Camera(Transform* parent) {
		transform = Transform(parent);
	}
	Camera() {
		transform = Transform();
	}
	Transform transform;
	const mat4 ProjectionMatrix = glm::perspective(glm::radians(70.f), 640.f / 480.f, 0.01f, 50000.f);
	vec2 xRotClamp{-90.f, 90.f };
};

