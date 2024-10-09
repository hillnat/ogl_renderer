#pragma once
#include <vector>
#include "rigidbody.h"
#include "renderer.h"
//const vec3 gravity = vec3(0, -9.807f, 0);//Earth gravity
const vec3 gravity = vec3(0, -0.01f, 0);//Earth gravity

class physics
{
public:
	physics() {
		activeBodies.clear();
	}
	std::vector<rigidbody*> activeBodies{};
	rigidbody CreateRigidbody(Transform* attachedBody);
	void UpdateAllBodies();
	bool SphereSphereOverlap(vec3 posA, float radA, vec3 posB, float radB);
};

