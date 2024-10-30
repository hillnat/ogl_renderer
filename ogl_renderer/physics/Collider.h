#pragma once

#include "ColliderShapes.h"
#include "../engine/Transform.h"

class Collider
{
public:
	Collider(ColliderShapes colliderShape) {
		this->colliderShape = colliderShape;
	}
	ColliderShapes colliderShape=ColliderShapes::Sphere;
};

