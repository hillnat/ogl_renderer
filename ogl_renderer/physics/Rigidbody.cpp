#include "Rigidbody.h"



void Rigidbody::UpdateBody(float fixedDeltaTime) {
	if(useGravity){
		AddForcePure(vec3(0, -4.9035f, 0));
	}
	attachedBody->TranslateGlobal(velocity* fixedDeltaTime);
}
void Rigidbody::AddForce(vec3 force) {
	velocity += (force / mass);
}
void Rigidbody::AddForcePure(vec3 pureForce) {
	velocity += pureForce;
}