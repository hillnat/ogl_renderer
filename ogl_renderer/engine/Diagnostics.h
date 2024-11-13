#pragma once
#include <iostream>
#include "glew/glew.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include <iostream>
#include <fstream> //File stream
#include <sstream> //String stream for reading entire files
#include <string>
#include <vector>
#define _CRT_SECURE_NO_WARNINGS //Allow old time functions
#include <ctime> //For time functions
#include <winsock2.h> //Provides functions and definitions for network programming using Winsock
#include <thread>

using namespace std;
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

	//Util
	void SetColor(int color);
	void SetColorDefault();
	void ClearLine(int count);
	void LogBlockLine(int count);
	void Log(char* message);
	void Log(string message);
	void Log(string message, int color);
};

