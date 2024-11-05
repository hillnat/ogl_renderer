#include "Physics.h"

void Physics::AddColRbPair(ColRbPair* crp) {
	colRbPairs.push_back(crp);
}
void Physics::UpdateAllBodies(float fixedDeltaTime) {
	//Tell RBs to apply their velocity
	if (colRbPairs.size() == 0) { return; }
	for (int i = 0; i < colRbPairs.size(); i++) {
		if ((*colRbPairs[i]).attachedRigidbody==nullptr) { continue; }
		if ((*colRbPairs[i]).attachedRigidbody->isStatic) { continue; }
		(*colRbPairs[i]).attachedRigidbody->UpdateBody(fixedDeltaTime);
	}
	//Collision checks
	for (int i = 0; i < colRbPairs.size()-1; i++) {//Loop with logic to avoid double checking

		Collider* colA = colRbPairs[i]->attachedCollider;
		if (colA == nullptr) { continue; }

		for (int k = i+1; k < colRbPairs.size(); k++) {
			if (i == k) { continue; }//Cant collide with self

			Collider* colB = colRbPairs[k]->attachedCollider;
			if (colB == nullptr) { continue; }
			
			switch (colA->colliderShape) {
				case ColliderShapes::Sphere:
					switch (colB->colliderShape) {
						case ColliderShapes::Sphere:
							if (OverlapSphereSphere(colRbPairs[i]->attachedRigidbody->attachedTransform->GetPosition(), 0.5f, colRbPairs[k]->attachedRigidbody->attachedTransform->GetPosition(), 0.5f)) {
								ResolveSphereSphere(colRbPairs[i]->attachedRigidbody, colRbPairs[k]->attachedRigidbody);
							}
							break;
						case ColliderShapes::Plane:
							if (OverlapSpherePlane(colRbPairs[i]->attachedRigidbody->attachedTransform->GetPosition(), 0.5f, colRbPairs[k]->attachedRigidbody->attachedTransform->GetPosition(), colRbPairs[k]->attachedRigidbody->attachedTransform->Up(), *colRbPairs[i]->attachedRigidbody)) {
								ResolveSpherePlane(colRbPairs[i]->attachedRigidbody, colRbPairs[k]->attachedRigidbody);
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
bool Physics::OverlapSphereSphere(const vec3 posA, const float radA, const vec3 posB, const float radB) {
	float dist = sqrtf(
		(posA.x - posB.x) * (posA.x - posB.x) +
		(posA.y - posB.y) * (posA.y - posB.y) +
		(posA.z - posB.z) * (posA.z - posB.z));
	return ((radA + radB) > dist);
}
bool Physics::OverlapSpherePlane(const vec3 spherePos, const float sphereRadius, const vec3 planePos, const vec3 planeNormal) {
	//Collision between a sphere and an infinite-depth plane

	vec3 dist = vec3(
		(spherePos.x - planePos.x),
		(spherePos.y - planePos.y),
		(spherePos.z - planePos.z));
	vec3 distNorm = dist * planeNormal;
	float mag = distNorm.x + distNorm.y + distNorm.z;
	bool isPenetrating = (mag < sphereRadius);
	return isPenetrating;
}
bool Physics::OverlapSpherePlane(const vec3 spherePos, const float sphereRadius, const vec3 planePos, const vec3 planeNormal, Rigidbody rbToDepenetrate) {
	//Collision between a sphere and an infinite-depth plane

	vec3 dist = vec3(
		(spherePos.x - planePos.x),
		(spherePos.y - planePos.y),
		(spherePos.z - planePos.z));
	vec3 distNorm = dist * planeNormal;
	float mag = distNorm.x + distNorm.y + distNorm.z;
	bool isPenetrating = (mag < sphereRadius);
	if (isPenetrating) {
		float penAmount = sphereRadius - mag;
		vec3 oldPos = rbToDepenetrate.attachedTransform->GetPosition();
		rbToDepenetrate.attachedTransform->TranslateGlobal(planeNormal * penAmount);
		vec3 newPos = rbToDepenetrate.attachedTransform->GetPosition();
		std::cout << "Depenetrated | X : " << abs(oldPos.x - newPos.x) << " Y : " << abs(oldPos.y - newPos.y) << " Z : " << abs(oldPos.z - newPos.z) << std::endl;
	}
	return isPenetrating;
}
bool Physics::OverlapSphereAABB(const vec3 spherePos, const float sphereRadius, const AABB aabb) {
	//Get point on edge of AABB
	vec3 point = GetClosestPointToAABB(spherePos, aabb);
	//Sphere to (point = sphere where r=0)
	return OverlapSphereSphere(spherePos, sphereRadius, point, 0);
}
#pragma endregion

#pragma region Collision Resolution
//Resolve collision between two RBs assumed as spheres.
void Physics::ResolveSphereSphere(Rigidbody* sphereA, Rigidbody* sphereB) {
	if (sphereA->isStatic || sphereB->isStatic) {//If either are static, set flip vel of whichever is not static
		if (!sphereA->isStatic) {
			sphereA->velocity *= -1.F;		
		}if (!sphereB->isStatic) { 
			sphereB->velocity *= -1.F;
		}
		return;
	}
	vec3 relVel = sphereA->velocity - sphereB->velocity;
	vec3 colNorm = normalize(sphereA->attachedTransform->GetPosition() - sphereB->attachedTransform->GetPosition());

	float scaledMassSum = ((1.f / sphereA->mass) + (1.f / sphereB->mass));
	float joules = dot(2.f * relVel, colNorm) / dot(colNorm, colNorm * scaledMassSum);

	sphereA->AddForce(-joules * colNorm);
	sphereB->AddForce(joules * colNorm);
}
//Resolve collision between an assumed sphere RB, and a assumed plane RB
void Physics::ResolveSpherePlane(Rigidbody* sphere, Rigidbody* plane) {
	if (sphere->isStatic || plane->isStatic) {//If either are static, set flip vel of whichever is not static
		if (!sphere->isStatic) {
			sphere->velocity *= -1.F;
		}if (!plane->isStatic) {
			plane->velocity *= -1.F;
		}
		return;
	}
	vec3 relVel = sphere->velocity - plane->velocity;
	vec3 colNorm = normalize(sphere->attachedTransform->GetPosition() - plane->attachedTransform->GetPosition());

	float scaledMassSum = ((1.f / sphere->mass) + (1.f / plane->mass));
	float joules = dot(2.f * relVel, colNorm) / dot(colNorm, colNorm * scaledMassSum);
	sphere->AddForce(-joules * colNorm);
	plane->AddForce(joules * colNorm);
}
#pragma endregion
#pragma region Point Checks
vec3 Physics::GetClosestPointToSphere(vec3 spherePos, float sphereRadius, vec3 targetPoint) {//Direction from sphere center to target point, times sphere radius
	vec3 dir = glm::normalize(targetPoint - spherePos);
	return spherePos + (dir * sphereRadius);//Return the position of the sphere, plus our direction times radius
}
vec3 Physics::GetClosestPointToAABB(vec3 targetPoint, AABB aabb) {
	//Returns a point laying on the edge of the AABB in the direction of the targetPoint.
	vec3 output = targetPoint;

	if (targetPoint.x > aabb.pos.x + (aabb.size.x / 2.F)) {
		output.x = aabb.pos.x + (aabb.size.x / 2.F);
	}
	else {
		output.x = targetPoint.x;
	}

	if (targetPoint.y > aabb.pos.y + (aabb.size.y / 2.F)) {
		output.y = aabb.pos.y + (aabb.size.y / 2.F);
	}
	else {
		output.y = targetPoint.y;
	}

	if (targetPoint.z > aabb.pos.z + (aabb.size.z / 2.F)) {
		output.z = aabb.pos.z + (aabb.size.z / 2.F);
	}
	else {
		output.z = targetPoint.z;
	}
	return output;
}
#pragma endregion