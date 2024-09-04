#pragma once
#include "glm/glm.hpp"//Math lib
#include "renderer.h"
class MeshImporter
{
public:
	Geometry ImportFromFile(const char* filename);
};

