#include "Rigidbody.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions



void Rigidbody::UpdateBody(float fixedDeltaTime) {
	

	if (!isResting) {
		//if (glm::length(velocity) < restingVelThreshold) {
		//	isResting = true;
		//	return;
		//}
		if (gravityScale != 0) {
			AddForcePure(vec3(0, -4.9035f * gravityScale, 0));
		}
		attachedTransform->TranslateGlobal(velocity * fixedDeltaTime);
		attachedTransform->Rotate(torque, fixedDeltaTime*glm::length(torque));//Multiply by the magnitude of the torque, because the torque axis gets normalized
	}	
}
void Rigidbody::AddForce(vec3 force) {
	AddForcePure(force / mass);
}
void Rigidbody::AddForcePure(vec3 pureForce) {
	if (isResting && glm::length(pureForce) >= restingVelThreshold) { isResting = false; }
	if (!isResting) { velocity += pureForce; }
}
void Rigidbody::AddTorque(vec3 torque) {
	AddForcePure(torque / mass);
}
void Rigidbody::AddTorquePure(vec3 pureTorque) {
	if (isResting && glm::length(pureTorque) >= restingVelThreshold) { isResting = false; }
	if (!isResting) { velocity += pureTorque; }
}