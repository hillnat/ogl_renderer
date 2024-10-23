#include "physics.h"
#include "transform.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include "ColliderShapes.h"
#include "ColRbPair.h"

void Physics::AddColRbPair(ColRbPair* crp) {
	colRbPairs.push_back(crp);
}
void Physics::UpdateAllBodies(float fixedDeltaTime) {
	//Tell RBs to apply their velocity
	if (colRbPairs.size() == 0) { return; }
	for (int i = 0; i < colRbPairs.size(); i++) {
		if ((*colRbPairs[i]).rigidbody==nullptr) { continue; }
		if ((*colRbPairs[i]).rigidbody->isStatic) { continue; }
		(*colRbPairs[i]).rigidbody->UpdateBody(fixedDeltaTime);
	}
	//Collision checks
	for (int i = 0; i < colRbPairs.size()-1; i++) {//Loop with logic to avoid double checking

		Collider* colA = colRbPairs[i]->collider;
		if (colA == nullptr) { continue; }

		for (int k = i+1; k < colRbPairs.size(); k++) {
			if (i == k) { continue; }//Cant collide with self

			Collider* colB = colRbPairs[k]->collider;
			if (colB == nullptr) { continue; }
			
			switch (colA->colliderShape) {
				case ColliderShapes::Sphere:
					switch (colB->colliderShape) {
						case ColliderShapes::Sphere:
							if (Sphere2Overlap(colRbPairs[i]->rigidbody->attachedBody->GetPosition(), 0.5f, colRbPairs[k]->rigidbody->attachedBody->GetPosition(), 0.5f)) {
								ResolveSphereSphere(colRbPairs[i]->rigidbody, colRbPairs[k]->rigidbody);
							}
							break;
						case ColliderShapes::Plane:
							if (Sphere2Overlap(colRbPairs[i]->rigidbody->attachedBody->GetPosition(), 0.5f, colRbPairs[k]->rigidbody->attachedBody->GetPosition(), 0.5f)) {
								ResolveSphereSphere(colRbPairs[i]->rigidbody, colRbPairs[k]->rigidbody);
							}
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}	
		}
	}
}
#pragma region Collision Detection
bool Physics::Sphere2Overlap(const vec3 posA, const float radA, const vec3 posB, const float radB) {
	float dist = sqrtf(
		(posA.x - posB.x) * (posA.x - posB.x) +
		(posA.y - posB.y) * (posA.y - posB.y) +
		(posA.z - posB.z) * (posA.z - posB.z));
	return ((radA + radB) > dist);
}
bool Physics::SpherePlaneOverlap(const vec3 spherePos, const float sphereRadius, const vec3 planePos, const vec3 planeNormal) {
	vec3 dist = vec3(
		(spherePos.x - planePos.x),
		(spherePos.y - planePos.y),
		(spherePos.z - planePos.z));
	vec3 distNorm = dist * planeNormal;
	float mag = distNorm.x + distNorm.y + distNorm.z;
	return (mag <sphereRadius);
}
#pragma endregion
#pragma region Collision Resolution
void Physics::ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb) {
	if (rba->isStatic || rbb->isStatic) {
		//If either are static, set flip vel of whichever is not static
		if (!rba->isStatic) { rba->velocity *= -1.F; }
		if (!rbb->isStatic) { rbb->velocity *= -1.F; }
		return;
	}
	vec3 relVel = rba->velocity - rbb->velocity;
	vec3 colNorm = normalize(rba->attachedBody->GetPosition() - rbb->attachedBody->GetPosition());
	//float elastFactor = 1;
	//float elasticity = -(1 + elastFactor);
	float scaledMassSum = ((1.f / rba->mass) + (1.f / rbb->mass));
	float joules = dot(2.f * relVel, colNorm) / dot(colNorm, colNorm * scaledMassSum);

	rba->AddForce(-joules * colNorm);
	rbb->AddForce(joules * colNorm);
}
void Physics::ResolveSpherePlane(Rigidbody* sphere, Rigidbody* plane) {
	if (sphere->isStatic || plane->isStatic) {
		//If either are static, set flip vel of whichever is not static
		if (!sphere->isStatic) { sphere->velocity *= -1.F; }
		if (!plane->isStatic) { plane->velocity *= -1.F; }
		return;
	}
	vec3 relVel = sphere->velocity - plane->velocity;
	vec3 colNorm = normalize(sphere->attachedBody->GetPosition() - plane->attachedBody->GetPosition());
	//float elastFactor = 1;
	//float elasticity = -(1 + elastFactor);
	float scaledMassSum = ((1.f / sphere->mass) + (1.f / plane->mass));
	float joules = dot(2.f * relVel, colNorm) / dot(colNorm, colNorm * scaledMassSum);
	sphere->AddForce(-joules * colNorm);
	plane->AddForce(joules * colNorm);
}

#pragma endregion