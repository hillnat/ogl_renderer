#pragma once
#include "glm/ext.hpp"

class Context
{
	struct GLFWwindow* window;
public:
	bool init(int width, int height, const char* title);
	void tick();//Update for the window
	void clear();//Clear the window
	void terminate();
	bool shouldClose();
	bool W_Pressed();
	bool S_Pressed();
	bool D_Pressed();
	bool A_Pressed();
	bool Q_Pressed();
	bool E_Pressed();
	bool Space_Pressed();
	bool LCtrl_Pressed();
	glm::dvec2 GetMouseDelta();
	glm::dvec2 lastMousePos = glm::vec2(0.f, 0.f);
};

