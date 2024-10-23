#pragma once
#include <vector>
#include "rigidbody.h"
#include "renderer.h"
#include "Collider.h"
#include "ColRbPair.h"
//const vec3 gravity = vec3(0, -9.807f, 0);//Earth gravity
const vec3 gravity = vec3(0, -4.9035f, 0);//Test gravity

class Physics
{
public:
	Physics() {
		colRbPairs.clear();
	}
	std::vector<ColRbPair*> colRbPairs{};
	//void AddColRbPair(Collider* c, Rigidbody* r);
	void AddColRbPair(ColRbPair* crp);
	void UpdateAllBodies(float fixedDeltaTime);
	bool Sphere2Overlap(vec3 posA, float radA, vec3 posB, float radB);
	bool SpherePlaneOverlap(vec3 spherePos, float sphereRadius, vec3 planePos, vec3 planeNormal);
	void ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb);
	void ResolveSpherePlane(Rigidbody* sphere, Rigidbody* plane);
};

