#include "context.h"
#define GLEW_STATIC//If proprocessor not defined, Sets the flag for GLEW to act static
#include "glew/glew.h"//GLEW helps us setup graphics pipeline and interface with hardware
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <random>
#include "glm/ext.hpp"
#include <iostream>

bool Context::Initialize(){
	glfwInit();
	window = glfwCreateWindow((int)TargetWindowSize.x, (int)TargetWindowSize.y, "Newton's Blender", NULL, NULL);
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
	glClearColor(0.757F, 0.714F, 0.91F, 1);
	std::srand(time(nullptr));
	return true;
}
void Context::Tick(){
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
void Context::ClearScreen(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear those two buffers every time we call clear
}
void Context::Terminate(){
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
}
bool Context::ShouldClose(){
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT);
}
bool Context::W_Pressed() { return glfwGetKey(window, GLFW_KEY_W); }
bool Context::S_Pressed() { return glfwGetKey(window, GLFW_KEY_S); }
bool Context::D_Pressed() { return glfwGetKey(window, GLFW_KEY_D); }
bool Context::A_Pressed() { return glfwGetKey(window, GLFW_KEY_A); }
bool Context::E_Pressed() { return glfwGetKey(window, GLFW_KEY_E); }
bool Context::Q_Pressed() { return glfwGetKey(window, GLFW_KEY_Q); }
bool Context::Space_Pressed() { return glfwGetKey(window, GLFW_KEY_SPACE); }
bool Context::LCtrl_Pressed() { return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL); }
bool Context::UpArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_UP); }
bool Context::RightArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_RIGHT); }
bool Context::LeftArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_LEFT); }
bool Context::DownArrow_Pressed() { return glfwGetKey(window, GLFW_KEY_DOWN); }
bool Context::Alpha1_Pressed() { return glfwGetKey(window, GLFW_KEY_1); }
bool Context::Mouse1_Pressed() { return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1); }

glm::dvec2 Context::GetMouseDelta(){
	glm::dvec2 curMousePos(0.f,0.f);
	glfwGetCursorPos(window, &curMousePos.x, &curMousePos.y);
	glm::dvec2 toReturn = curMousePos - lastMousePos;
	//std::cout << toReturn.x << " " << toReturn.y << std::endl;
	lastMousePos = curMousePos;
	return toReturn;
}