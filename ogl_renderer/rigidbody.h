#pragma once
#include "transform.h"
class Rigidbody
{
public:
#pragma region Constructors
	Rigidbody(Transform* attachedBody) {
		this->attachedBody = attachedBody;
		this->velocity = vec3(0, 0, 0);
		this->useGravity = true;
		this->mass = 1;
	}
	Rigidbody(Transform* attachedBody, vec3 vel, bool useGravity, float mass) {
		this->attachedBody = attachedBody;
		this->velocity = vel;
		this->useGravity = useGravity;
		this->mass = mass;
	}

#pragma endregion
	Transform* attachedBody;
	bool useGravity = true;
	vec3 velocity = vec3(0, 0, 0);
	float mass=1;
	void UpdateBody(float fixedDeltaTime);
	void AddForce(vec3 force);
};

