#pragma once
#include "glm/ext.hpp"

class Context
{
public:
	struct GLFWwindow* window;
	/// <summary> Initiliazes the context window, and additional GPU setup</summary>
	/// <returns> If successful </returns>
	bool Initialize();
	void Tick();//Update for the window
	void ClearScreen();//Clear the window
	void Terminate();
	/// <returns> If the user has attempted to exit the window</returns>
	bool ShouldClose();
	/// <returns> If the W key is currently held </returns>
	bool W_Pressed();
	/// <returns> If the S key is currently held </returns>
	bool S_Pressed();
	/// <returns> If the D key is currently held </returns>
	bool D_Pressed();
	/// <returns> If the A key is currently held </returns>
	bool A_Pressed();
	/// <returns> If the Q key is currently held </returns>
	bool Q_Pressed();
	/// <returns> If the E key is currently held </returns>
	bool E_Pressed();
	/// <returns> If the Space key is currently held </returns>
	bool Space_Pressed();
	/// <returns> If the Left Ctrl is currently held </returns>
	bool LCtrl_Pressed();
	/// <returns> If the Up Arrow is currently held </returns>
	bool UpArrow_Pressed();
	/// <returns> If the Right Arrow key is currently held </returns>
	bool RightArrow_Pressed();
	/// <returns> If the Left Arrow key is currently held </returns>
	bool LeftArrow_Pressed();
	/// <returns> If the Down Arrrow key is currently held </returns>
	bool DownArrow_Pressed();
	/// <returns> If the Mouse 1 button is currently held </returns>
	bool Mouse1_Pressed();
	/// <returns> Mouse movement </returns>
	glm::dvec2 GetMouseDelta();
	glm::vec2 TargetWindowSize{ 1600,900 };
	glm::dvec2 lastMousePos = glm::vec2(0.f, 0.f);
};

