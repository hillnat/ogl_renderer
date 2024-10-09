#pragma once
#include "renderer.h"
enum ShapeType {
	Empty = -1,
	Plane = 0,
	Line = 1,
	Box = 2,
	Cylinder = 3,
	Capsule = 4,
	Sphere = 5
};
const vec4 RED = vec4{ 1.f,0.f,0.f,1.f };
const vec4 BLUE = vec4{ 0.f,0.f,1.f,1.f };
const vec4 GREEN = vec4{ 0.f,1.f,0.f,1.f };
const vec4 YELLOW = vec4{ 1.f,1.f,0.f,1.f };
const vec4 MAGENTA = vec4{ 1.f,0.f,1.f,1.f };
const vec4 CYAN = vec4{ 0.f,1.f,1.f,1.f };
const vec4 GREY = vec4{ 0.5f,0.5f,0.5f,1.f };
const vec4 BLACK = vec4{ 1.f,0.f,0.f,1.f };
const vec4 WHITE = vec4{ 1.f,1.f,1.f,1.f };
mesh MakeCube();
mesh MakeCube();
mesh MakeSphere();
mesh MakeCylinder();
mesh MakePlane();

