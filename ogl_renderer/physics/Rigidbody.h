#pragma once
#include "../engine/Transform.h"
#include "ColliderShapes.h"

class Rigidbody
{
private:
	const float restingVelThreshold = 0.5f;
public:
#pragma region Constructors
	Rigidbody(Transform* attachedBody) {
		this->attachedTransform = attachedBody;
		this->velocity = vec3(0, 0, 0);
		this->gravityScale = 0;
		this->mass = 1;
		this->isStatic = false;
		this->isResting = false;
	}
	Rigidbody(Transform* attachedBody, vec3 vel, float gravityScale, bool isStatic,float mass) {
		this->attachedTransform = attachedBody;
		this->velocity = vel;
		this->gravityScale = gravityScale;
		this->mass = mass;
		this->isStatic = isStatic;
		this->isResting = false;
	}
#pragma endregion
	Transform* attachedTransform;
	float gravityScale = 0;
	vec3 velocity = vec3(0, 0, 0);
	float mass=1;
	bool isStatic = false;
	bool isResting = false;
	/// <summary> Executes a physics tick on this object </summary>
	/// <param name="fixedDeltaTime">The current fixed delta time</param>
	void UpdateBody(float fixedDeltaTime);
	/// <summary> Adds force, factoring in mass </summary>
	/// <param name="force">Amount of force to apply</param>
	void AddForce(vec3 force);
	/// <summary> Adds force, ignoring mass </summary>
	/// <param name="pureForce">Amount of force to apply</param>
	void AddForcePure(vec3 pureForce);
};

