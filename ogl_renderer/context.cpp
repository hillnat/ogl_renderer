#include "context.h"
#define GLEW_STATIC//If proprocessor not defined, Sets the flag for GLEW to act static
#include "glew/glew.h"//GLEW helps us setup graphics pipeline and interface with hardware
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <random>
#include "glm/ext.hpp"
#include <iostream>
bool context::Initialize(){
	glfwInit();
	window = glfwCreateWindow((int)TargetWindowSize.x, (int)TargetWindowSize.y, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	//Set flags for opengl featues! OpenGL checks ifdef on these flags
	glEnable(GL_BLEND);//Enable blend
	glEnable(GL_DEPTH_TEST);//Enable depth
	glEnable(GL_CULL_FACE);//Optimazation to only render 1 face
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Selected blending function
	glDepthFunc(GL_LEQUAL);//Decide which objects are in front
	glFrontFace(GL_CCW);//Algorithm for normal. Winding direction for determining normal direction on triangle
	glCullFace(GL_BACK);//Which side should we hide? NOTE : THIS SHOULD BE CULLING BACKFACE (GL_BACK)
	glClearColor(0.109f, 0.21f, 0.45f, 1);
	std::srand(time(nullptr));
	return true;
}
void context::Tick(){
	glfwSwapBuffers(window);
	glfwPollEvents();//tick polling
	//Free crosshair
	if(glfwGetKey(window, GLFW_KEY_MINUS))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	//glfwSetWindowSize(window, std::rand()%1000, std::rand() % 1000);
}
void context::Clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear those two buffers every time we call clear
}
void context::Terminate(){
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
}
bool context::ShouldClose(){
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT);
}
bool context::W_Pressed() { return glfwGetKey(window, GLFW_KEY_W); }
bool context::S_Pressed() { return glfwGetKey(window, GLFW_KEY_S); }
bool context::D_Pressed() { return glfwGetKey(window, GLFW_KEY_D); }
bool context::A_Pressed() { return glfwGetKey(window, GLFW_KEY_A); }
bool context::E_Pressed() { return glfwGetKey(window, GLFW_KEY_E); }
bool context::Q_Pressed() { return glfwGetKey(window, GLFW_KEY_Q); }
bool context::Space_Pressed() { return glfwGetKey(window, GLFW_KEY_SPACE); }
bool context::LCtrl_Pressed() { return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL); }
bool context::UpArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_UP); }
bool context::RightArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_RIGHT); }
bool context::LeftArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_LEFT); }
bool context::DownArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_DOWN); }
glm::dvec2 context::GetMouseDelta(){
	glm::dvec2 curMousePos(0.f,0.f);
	glfwGetCursorPos(window, &curMousePos.x, &curMousePos.y);
	glm::dvec2 toReturn = curMousePos - lastMousePos;
	//std::cout << toReturn.x << " " << toReturn.y << std::endl;
	lastMousePos = curMousePos;
	return toReturn;
}