#pragma once
#include "transform.h"
class rigidbody
{
public:
#pragma region Constructors
	rigidbody(Transform* attachedBody) {
		this->attachedBody = attachedBody;
		this->velocity = vec3(0, 0, 0);
		this->useGravity = true;
	}
	rigidbody(Transform* attachedBody, vec3 vel) {
		this->attachedBody = attachedBody;
		this->velocity = vel;
		this->useGravity = true;
	}
	rigidbody(Transform* attachedBody, vec3 vel, bool useGravity) {
		this->attachedBody = attachedBody;
		this->velocity = vel;
		this->useGravity = useGravity;
	}
	rigidbody(Transform* attachedBody, bool useGravity) {
		this->attachedBody = attachedBody;
		this->velocity = vec3(0,0,0);
		this->useGravity = useGravity;
	}
#pragma endregion
	Transform* attachedBody;
	bool useGravity = true;
	vec3 velocity = vec3(0, 0, 0);
	void UpdateBody();
};

