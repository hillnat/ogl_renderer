#pragma once
#include <vector>
#include "Rigidbody.h"
#include "ColRbPair.h"
#include "../engine/Transform.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
//const vec3 gravity = vec3(0, -9.807f, 0);//Earth gravity

class Physics {
public:
	Physics() {
		colRbPairs.clear();
	}
	std::vector<ColRbPair*> colRbPairs{};
	/// <summary>
	/// Add a ColRbPair to our vector, allowing it to collide.
	/// </summary>
	/// <param name="crp">The ColRbPair to add</param>
	void AddColRbPair(ColRbPair* crp);
	/// <summary>
	/// Executes the physics tick on all physics objects in vector
	/// </summary>
	/// <param name="fixedDeltaTime">Current fixed delta time</param>
	void UpdateAllBodies(float fixedDeltaTime);
	/// <summary>
	/// Checks of two spheres are overlapping
	/// </summary>
	/// <param name="posA">Position of sphere A</param>
	/// <param name="radA">Radius of sphere A</param>
	/// <param name="posB">Position of sphere B</param>
	/// <param name="radB">Radius of sphere B</param>
	/// <returns></returns>
	bool OverlapSphereSphere(vec3 posA, float radA, vec3 posB, float radB);
	/// <summary>
	/// Checks if a sphere is overlapping a plane
	/// </summary>
	/// <param name="spherePos">Positon of the sphere</param>
	/// <param name="sphereRadius">Radius of the sphere</param>
	/// <param name="planePos">Position of the plane</param>
	/// <param name="planeNormal">Normal of the plane</param>
	/// <returns>If overlapping</returns>
	bool OverlapSpherePlane(vec3 spherePos, float sphereRadius, vec3 planePos, vec3 planeNormal);
	/// <summary>
	/// Checks if a sphere is overlapping a plane, and depenetrates
	/// </summary>
	/// <param name="spherePos">Position of the sphere</param>
	/// <param name="sphereRadius">Radius of the sphere</param>
	/// <param name="planePos">Position of the plane</param>
	/// <param name="planeNormal">Normal of the plane</param>
	/// <param name="rbToDepenetrate">Which rb should be depenetrated?</param>
	/// <returns>If overlapping</returns>
	bool OverlapSpherePlane(vec3 spherePos, float sphereRadius, vec3 planePos, vec3 planeNormal, Rigidbody rbToDepenetrate);

	//bool OverlapSphereBox(const vec3 spherePos, const float sphereRadius, const vec3 boxPos, const vec3 boxSize);

	/// <summary>
	/// Check if a sphere is overlapping an AABB
	/// </summary>
	/// <param name="spherePos">Position of the sphere</param>
	/// <param name="sphereRadius">Radius of the sphere</param>
	/// <param name="aabb">The AABB</param>
	/// <returns>If successful</returns>
	bool OverlapSphereAABB(const vec3 spherePos, const float sphereRadius, const AABB aabb);
	/// <summary>
	/// Resolves a collision between two sphere Rigidbodies
	/// </summary>
	/// <param name="rba"></param>
	/// <param name="rbb"></param>
	void ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb);
	/// <summary>
	/// Resolves a collision between one sphere RigidBody and one plane RigidBody
	/// </summary>
	/// <param name="sphere">The sphere's rigidbody</param>
	/// <param name="plane">The plane's rigibody</param>
	void ResolveSpherePlane(Rigidbody* sphere, Rigidbody* plane);
	/// <summary>
	/// Gets a point within a sphere that is closest to target point
	/// </summary>
	/// <param name="spherePos">Position of sphere</param>
	/// <param name="sphereRadius">Radius of sphere</param>
	/// <param name="targetPoint">Point to evalutate</param>
	/// <returns>The closest point to targetPoint in sphere</returns>
	vec3 GetClosestPointToSphere(vec3 spherePos, float sphereRadius, vec3 targetPoint);
	/// <summary>
	/// Gets a point within an AABB that is closest to target point
	/// </summary>
	/// <param name="targetPoint">The point we are evaluating</param>
	/// <param name="aabb">The AABB to use</param>
	/// <returns>The closest point to targetPoint in aabb</returns>
	vec3 GetClosestPointToAABB(vec3 targetPoint, AABB aabb);
};
