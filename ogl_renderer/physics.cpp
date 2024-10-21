#include "physics.h"
#include "transform.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions

void Physics::AddRigidbody(Rigidbody r) {
	activeBodies.push_back(&r);
}
void Physics::UpdateAllBodies(float fixedDeltaTime) {//Need to fix double collisions
	//std::cout << "Physics::UpdateAllBodies, activeBodies.size() = " << activeBodies.size() << std::endl;
	for (int i = 0; i < activeBodies.size(); i++) {
		if ((*activeBodies[i]).isStatic) { continue; }
		(*activeBodies[i]).UpdateBody(fixedDeltaTime);
	}
	for (int i = 0; i < activeBodies.size()-1; i++) {
		for (int k = i+1; k < activeBodies.size(); k++) {
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
	if (rba->isStatic || rbb->isStatic) {
		rba->velocity *= -1.F;
		rbb->velocity *= -1.F;
		return;
	}
	vec3 relVel = rba->velocity - rbb->velocity;
	vec3 colNorm = normalize(rba->attachedBody->GetPosition() - rbb->attachedBody->GetPosition());
	//float elastFactor = 1;
	//float elasticity = -(1 + elastFactor);
	float scaledMassSum = ((1.f / rba->mass) + (1.f / rbb->mass));
	float joules = dot(2.f * relVel, colNorm) / dot(colNorm, colNorm * scaledMassSum);

	rba->velocity -= ((joules / rba->mass) * colNorm);
	rbb->velocity += ((joules / rbb->mass) * colNorm);
}