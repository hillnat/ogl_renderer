#pragma once
#include "glm/ext.hpp"

class Context
{
public:
	struct GLFWwindow* window;
	bool Initialize();
	void Tick();//Update for the window
	void Clear();//Clear the window
	void Terminate();
	bool ShouldClose();
	bool W_Pressed();
	bool S_Pressed();
	bool D_Pressed();
	bool A_Pressed();
	bool Q_Pressed();
	bool E_Pressed();
	bool Space_Pressed();
	bool LCtrl_Pressed();
	bool UpArrow_Pressed();
	bool RightArrow_Pressed();
	bool LeftArrow_Pressed();
	bool DownArrow_Pressed();
	glm::dvec2 GetMouseDelta();
	glm::vec2 TargetWindowSize{ 1600,900 };
	glm::dvec2 lastMousePos = glm::vec2(0.f, 0.f);
};

