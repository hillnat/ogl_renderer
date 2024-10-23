#include "rigidbody.h"
#include "physics.h"
void Rigidbody::UpdateBody(float fixedDeltaTime) {
	if(useGravity){
		AddForcePure(gravity);
	}
	attachedBody->TranslateGlobal(velocity* fixedDeltaTime);
}
void Rigidbody::AddForce(vec3 force) {
	velocity += (force / mass);
}
void Rigidbody::AddForcePure(vec3 pureForce) {
	velocity += pureForce;
}