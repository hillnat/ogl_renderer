#pragma once
enum class ColliderShapes { Sphere=0, Plane=1, Cube=2, Capsule=3, Ground=4, AABB=5 };
//1 : Sphere
//2 : Plane, 1 sided
//3 : Cube, 6 sets of 4 vertices
//4 : Capsule
//5 : Ground, plane which collider extendes infitley along the negative normal
//6 : Axis Aligned Bounding Box (AABB)