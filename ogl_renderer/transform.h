#pragma once
#include "glm/ext.hpp"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::quat;
using glm::identity;
#include <iostream>
class Transform
{	
public:
#pragma region Constructors
	Transform(vec3 pos, quat rot, vec3 scale, Transform* parent) {
		this->position = pos;
		this->rotation = rot;
		this->scale = scale;
		this->parent = parent;
	}
	Transform(Transform* parent) {
		this->position = vec3(0.f, 0.f, 0.f);
		this->rotation = identity<quat>();
		this->scale = vec3(1.f, 1.f, 1.f);
		this->parent = parent;
	}
	Transform() {
		this->position = vec3(0.f, 0.f, 0.f);
		this->rotation = identity<quat>();
		this->scale = vec3(1.f, 1.f, 1.f);
		this->parent = nullptr;
	}
#pragma endregion
	Transform* parent = nullptr;
	vec3 position = vec3(0.f, 0.f, 0.f);
	quat rotation = identity<quat>();
	vec3 scale=vec3(1.f,1.f,1.f);
#pragma region Axis
	vec3 right() {return glm::normalize(vec3(VecxQuat(rotation, vec4{ 1.f,0.f,0.f,1.f }))); }
	vec3 up() { return glm::normalize(vec3(VecxQuat(rotation, vec4{ 0.f,1.f,0.f,1.f }))); }
	vec3 forward() { return glm::normalize(vec3(VecxQuat(rotation, vec4{ 0.f,0.f,1.f,1.f }))); }
#pragma endregion
	quat GetCombinedRotation() { if (parent != nullptr) { return parent->GetCombinedRotation() * rotation; } else { return rotation; } }//Rotation factoring in our parent if applicable
	vec3 GetEulerAngles() {
		vec3 e = glm::eulerAngles(rotation);//radians by glm default
		
		return vec3{ glm::degrees(e.x),glm::degrees(e.y),glm::degrees(e.z) };
	}
	mat4 GetMatrix();//Compose matrix from raw vectors and quaternion
#pragma region Translation and Rotation
	void GlobalTranslate(vec3);
	void LocalTranslate(vec3);
	void LocalParentTranslate(vec3);
	void RotateEuler(vec3,float);
#pragma endregion
#pragma region Utilities
	vec4 VecxQuat(quat& q, vec4 v) {
		quat q2 = inverse(q);
		v = glm::normalize(v);
		return vec4(
			(q2.x * v.x) + (q2.y * v.x) + (q2.z * v.x) + (q2.w * v.x),
			(q2.x * v.y) + (q2.y * v.y) + (q2.z * v.y) + (q2.w * v.y),
			(q2.x * v.z) + (q2.y * v.z) + (q2.z * v.z) + (q2.w * v.z),
			(q2.x * v.w) + (q2.y * v.w) + (q2.z * v.w) + (q2.w * v.w)
		);
	}
#pragma endregion
};
