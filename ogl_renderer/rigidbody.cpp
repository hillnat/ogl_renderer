#include "rigidbody.h"
#include "physics.h"
void Rigidbody::UpdateBody(float fixedDeltaTime) {
	ApplyVelocity(velocity*fixedDeltaTime);
}
void Rigidbody::ApplyVelocity(vec3 force) {
	attachedBody->TranslateGlobal(force);
}