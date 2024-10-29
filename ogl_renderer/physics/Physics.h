#pragma once
#include <vector>
#include "Rigidbody.h"
#include "ColRbPair.h"
#include "../transform.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
//const vec3 gravity = vec3(0, -9.807f, 0);//Earth gravity
class Rigidbody;
class Physics
{
public:
	Physics() {
		colRbPairs.clear();
	}
	std::vector<ColRbPair*> colRbPairs{};
	void AddColRbPair(ColRbPair* crp);
	void UpdateAllBodies(float fixedDeltaTime);

	bool OverlapSphereSphere(vec3 posA, float radA, vec3 posB, float radB);
	bool OverlapSpherePlane(vec3 spherePos, float sphereRadius, vec3 planePos, vec3 planeNormal);
	//bool OverlapSphereBox(const vec3 spherePos, const float sphereRadius, const vec3 boxPos, const vec3 boxSize);
	bool OverlapSphereAABB(const vec3 spherePos, const float sphereRadius, const AABB aabb);

	void ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb);
	void ResolveSpherePlane(Rigidbody* sphere, Rigidbody* plane);

	vec3 GetClosestPointToSphere(vec3 spherePos, float sphereRadius, vec3 targetPoint);
	vec3 GetClosestPointToAABB(vec3 targetPoint, AABB aabb);
};
