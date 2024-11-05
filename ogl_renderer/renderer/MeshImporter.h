#pragma once
#include "glm/glm.hpp"//Math lib
#include "renderer.h"
namespace MeshImporter {
	/// <summary>
	/// Import a mesh from file
	/// </summary>
	/// <param name="filePath">File path to use</param>
	/// <returns>The imported mesh</returns>
	Mesh ImportFromFile(const char* filePath);
}

