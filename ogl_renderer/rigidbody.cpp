#include "rigidbody.h"
#include "physics.h"
void rigidbody::UpdateBody() {
	vec3 forceToApply = velocity;
	if (useGravity) { forceToApply += gravity; }

	attachedBody->Translate(forceToApply);
}