#include "diagnostics.h"
#include <iostream>
#include "glew/glew.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
using namespace std;
namespace Diagnostics {
	void LogHardware() {
		cout << "OpenGL Version : " << (const char*)glGetString(GL_VERSION) << endl;//Char pointer is const bc we are reading form a compiled lib / macro
		cout << "GLEW Version   : " << (const char*)glewGetString(GLEW_VERSION) << endl;
		cout << "GLSL Version   : " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
		cout << "GPU (Active)   : " << (const char*)glGetString(GL_RENDERER) << endl;
	}
	void CheckGLError()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << err << std::endl;
		}
	}
	void LogVec3(glm::vec3 vector) {
		cout  << " | X : " << vector.x << " Y : " << vector.y << " Z : " << vector.z << endl;
	}
}