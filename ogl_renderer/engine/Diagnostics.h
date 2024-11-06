#pragma once
#include "glm/ext.hpp" //GLM provides us with matrices and math functions

namespace Diagnostics
{
	/// <summary>
	/// Logs the current hardware to console
	/// </summary>
	void LogHardware();
	/// <summary>
	/// Checks for OpenGL error codes, and prints them
	/// </summary>
	void CheckGLError();

	void LogVec3(glm::vec3 vector);
};

