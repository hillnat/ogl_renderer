#include "ColliderShapes.h"
#include "transform.h"

#pragma once
class Collider
{
public:
	Collider(ColliderShapes colliderShape) {
		this->colliderShape = colliderShape;
	}
	ColliderShapes colliderShape=ColliderShapes::Sphere;
};

