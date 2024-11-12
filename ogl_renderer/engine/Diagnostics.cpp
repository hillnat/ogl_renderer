#include "Diagnostics.h"
#define _CRT_SECURE_NO_WARNINGS //Allow old time functions



void Diagnostics::LogHardware() {
	SetColor(FOREGROUND_GREEN);
	LogBlockLine(50);
	cout << "OpenGL Version : " << (const char*)glGetString(GL_VERSION) << endl;//Char pointer is const bc we are reading form a compiled lib / macro
	cout << "GLEW Version   : " << (const char*)glewGetString(GLEW_VERSION) << endl;
	cout << "GLSL Version   : " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << "GPU (Active)   : " << (const char*)glGetString(GL_RENDERER) << endl;
	LogBlockLine(50);
	SetColorDefault();
}
void Diagnostics::CheckGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}
}
void Diagnostics::LogVec3(glm::vec3 vector) {
	cout << " | X : " << vector.x << " Y : " << vector.y << " Z : " << vector.z << endl;
}

void Diagnostics::SetColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
void Diagnostics::SetColorDefault() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void Diagnostics::ClearLine(int count) {
	for (int i = 0;i < count;i++) {
		cout << "\033[A";//Move cursor up one line   
		cout << "\033[K";//Clear the line
	}
}
string Diagnostics::GetTimeAsString() {
	string output = "[";
	auto end = chrono::system_clock::now();
	std::time_t end_time = chrono::system_clock::to_time_t(end);
	//output += ctime(&end_time);
	output[output.length() - 1] = ']';//Setting last char removes the \n which is automatically added by ctime()
	output += " ";
	return output;
}

void Diagnostics::Log(char* message) {
	cout << message << endl;
}
void Diagnostics::Log(string message) {
	cout << message << endl;
}
void Diagnostics::Log(string message, int color) {
	SetColor(color);
	cout << message << endl;
	SetColorDefault();
}
void Diagnostics::LogBlockLine(int count) {
	for (int i = 0; i < count; i++) {
		cout<<(char)254;
	}
	cout << endl;
}