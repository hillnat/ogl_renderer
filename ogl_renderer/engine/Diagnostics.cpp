#include "Diagnostics.h"
#define _CRT_SECURE_NO_WARNINGS //Allow old time functions

namespace Diagnostics {

	void LogHardware() {
		SetColor(FOREGROUND_GREEN);
		cout << "OpenGL Version : " << (const char*)glGetString(GL_VERSION) << endl;//Char pointer is const bc we are reading form a compiled lib / macro
		cout << "GLEW Version   : " << (const char*)glewGetString(GLEW_VERSION) << endl;
		cout << "GLSL Version   : " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
		cout << "GPU (Active)   : " << (const char*)glGetString(GL_RENDERER) << endl;
		SetColorDefault();
	}
	void CheckGLError()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << err << std::endl;
		}
	}
	void LogVec3(glm::vec3 vector) {
		cout << " | X : " << vector.x << " Y : " << vector.y << " Z : " << vector.z << endl;
	}

	void SetColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}
	void SetColorDefault() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	void ClearLine(int count) {
		for (int i = 0;i < count;i++) {
			cout << "\033[A";//Move cursor up one line   
			cout << "\033[K";//Clear the line
		}
	}

	void Log(char* message) {
		cout << message << endl;
	}
	void Log(string message) {
		cout << message << endl;
	}
	void Log(string message, int color) {
		SetColor(color);
		cout << endl << message << endl;
		SetColorDefault();
	}
	void LogBlockLine(int count) {
		for (int i = 0; i < count; i++) {
			cout << (char)254;
		}
		cout << endl;
	}
}