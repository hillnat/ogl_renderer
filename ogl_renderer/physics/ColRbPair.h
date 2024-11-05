#pragma once
#include "rigidbody.h"
#include "Collider.h"
class Rigidbody;
struct ColRbPair {
	ColRbPair(Collider* c, Rigidbody* r) {
		this->attachedRigidbody = r;
		this->attachedCollider = c;
	}
	ColRbPair(Rigidbody* r) {
		this->attachedRigidbody = r;
	}
	ColRbPair(Collider* c) {
		this->attachedCollider = c;
	}

	Rigidbody* attachedRigidbody= nullptr;
	Collider* attachedCollider=nullptr;
};