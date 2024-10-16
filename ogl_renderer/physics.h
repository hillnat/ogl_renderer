#pragma once
#include <vector>
#include "rigidbody.h"
#include "renderer.h"
//const vec3 gravity = vec3(0, -9.807f, 0);//Earth gravity
const vec3 gravity = vec3(0, -0.01f, 0);//Earth gravity

class Physics
{
public:
	Physics() {
		activeBodies.clear();
	}
	std::vector<Rigidbody*> activeBodies{};
	Rigidbody CreateRigidbody(Transform* attachedBody, vec3 baseForce, bool useGravity, float mass);
	void UpdateAllBodies(float fixedDeltaTime);
	bool Sphere2Overlap(vec3 posA, float radA, vec3 posB, float radB);
	void ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb);
};

