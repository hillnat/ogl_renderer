#include "physics.h"
#include "transform.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions

Rigidbody Physics::CreateRigidbody(Transform* attachedBody, vec3 baseForce, bool useGravity, float mass) {
	Rigidbody r(attachedBody, baseForce, useGravity, mass);
	activeBodies.push_back(&r);
	return r;
}
void Physics::UpdateAllBodies(float fixedDeltaTime) {//Need to fix double collisions
	//std::cout << "Physics::UpdateAllBodies, activeBodies.size() = " << activeBodies.size() << std::endl;
	for (int i = 0; i < activeBodies.size(); i++) {
		(*activeBodies[i]).UpdateBody(fixedDeltaTime);
	}
	for (int i = 0; i < activeBodies.size(); i++) {
		for (int k = 0; k < activeBodies.size(); k++) {
			if (i == k) { continue; }//Cant collide with self
			if (Sphere2Overlap(activeBodies[i]->attachedBody->GetPosition(), 0.5f, activeBodies[k]->attachedBody->GetPosition(), 0.5f)) {
				ResolveSphereSphere(activeBodies[i], activeBodies[k]);
			}
		}
	}
}
bool Physics::Sphere2Overlap(const vec3 posA, const float radA, const vec3 posB, const float radB) {
	float dist = sqrtf(
		(posA.x - posB.x) * (posA.x - posB.x) +
		(posA.y - posB.y) * (posA.y - posB.y) +
		(posA.z - posB.z) * (posA.z - posB.z));
	return ((radA + radB) > dist);
}
void Physics::ResolveSphereSphere(Rigidbody* rba, Rigidbody* rbb) {
	vec3 relVel = rbb->velocity - rba->velocity;
	vec3 colNorm = normalize(rbb->attachedBody->GetPosition() - rba->attachedBody->GetPosition());
	//float elastFactor = 1;
	//float elasticity = -(1 + elastFactor);
	float scaledMassSum = ((1.f / rba->mass) + (1.f / rbb->mass));
	float joules = dot(-2.f * relVel, colNorm) / scaledMassSum;
	//float joules = dot(-2.f * relVel, colNorm) / dot(colNorm,colNorm*scaledMassSum);
	std::cout << "----- BEGIN PHYSICS -----" << std::endl
	<< "TYPE : ResolveSphereSphere" << std::endl
	<< "RbA Mass           : " << rba->mass << std::endl
	<< "RbB Mass           : " << rbb->mass << std::endl
	<< "RbA Velocity (IN)  : " << rba->velocity.x << " , " << rba->velocity.y << " , " << rba->velocity.z << std::endl
	<< "RbA Velocity (IN)  : " << rbb->velocity.x << " , " << rbb->velocity.y << " , " << rbb->velocity.z << std::endl;

	rba->velocity = rba->velocity + ((joules / rba->mass) * colNorm);
	rbb->velocity = rbb->velocity - ((joules / rbb->mass) * colNorm);

	vec3 relVel2 = rbb->velocity - rba->velocity;
	float joules2 = dot(-2.f * relVel2, colNorm) / scaledMassSum;
	std::cout
	<< "RbA Velocity (OUT) : " << rba->velocity.x << " , " << rba->velocity.y << " , " << rba->velocity.z << std::endl
	<< "RbA Velocity (OUT) : " << rbb->velocity.x << " , " << rbb->velocity.y << " , " << rbb->velocity.z << std::endl
	<< "Joules (IN)        : " << joules << std::endl
	<< "Joules (OUT)       : " << joules2 << std::endl
	<< "----- END PHYSICS -----" << std::endl;
}