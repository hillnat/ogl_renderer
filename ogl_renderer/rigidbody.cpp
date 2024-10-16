#include "rigidbody.h"
#include "physics.h"
void Rigidbody::UpdateBody(float fixedDeltaTime) {
	AddForce(velocity*fixedDeltaTime);
}
void Rigidbody::AddForce(vec3 force) {
	attachedBody->TranslateGlobal(force / mass);
}