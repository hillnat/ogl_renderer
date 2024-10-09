#include "physics.h"


rigidbody physics::CreateRigidbody(Transform* attachedBody) {
	rigidbody r(attachedBody);
	activeBodies.push_back(&r);
	return r;
}
void physics::UpdateAllBodies() {
	//std::cout << "Physics::UpdateAllBodies, activeBodies.size() = " << activeBodies.size() << std::endl;
	for (int i = 0; i < activeBodies.size(); i++) {
		(*activeBodies[i]).UpdateBody();
	}
}
bool physics::SphereSphereOverlap(const vec3 posA, const float radA, const vec3 posB, const float radB) {
	float dist = abs((
		abs(posA.x - posB.x) + 
		abs(posA.y - posB.y) + 
		abs(posA.z - posB.z)) 
		/ 3.f);//Average distance apart
	return ((radA + radB) > dist);//Return if the 
}