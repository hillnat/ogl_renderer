#pragma once
#include "rigidbody.h"
#include "Collider.h"
class Rigidbody;
struct ColRbPair {
	ColRbPair(Collider* c, Rigidbody* r) {
		this->rigidbody = r;
		this->collider = c;
	}
	ColRbPair(Rigidbody* r) {
		this->rigidbody = r;
	}
	ColRbPair(Collider* c) {
		this->collider = c;
	}
	Rigidbody* rigidbody= nullptr;
	Collider* collider=nullptr;
};