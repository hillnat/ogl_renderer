#include "context.h"
#define GLEW_STATIC//If proprocessor not defined, Sets the flag for GLEW to act static
#include "glew/glew.h"//GLEW helps us setup graphics pipeline and interface with hardware
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <random>
bool Context::init(int width, int height, const char* title){
	glfwInit();
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	//Set flags for opengl featues! OpenGL checks ifdef on these flags
	glEnable(GL_BLEND);//Enalbe blend
	glEnable(GL_DEPTH_TEST);//Enable depth
	glEnable(GL_CULL_FACE);//Optimazation to only render 1 face
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Selected blending function
	glDepthFunc(GL_LEQUAL);//Decide which objects are in front
	glFrontFace(GL_CCW);//Algorithm for normal. Winding direction for determining normal direction on triangle
	glCullFace(GL_BACK);
	glClearColor(0.109f, 0.21f, 0.45f, 1);
	std::srand(time(nullptr));
	return true;
}
void Context::tick(){
	glfwSwapBuffers(window);
	glfwPollEvents();//tick polling
	//glfwSetWindowSize(window, std::rand()%1000, std::rand() % 1000);
}
void Context::clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear those two buffers every time we call clear
}
void Context::terminate(){
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
}
bool Context::shouldClose(){
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}
bool Context::W_Pressed() { return glfwGetKey(window, GLFW_KEY_W); }
bool Context::S_Pressed() { return glfwGetKey(window, GLFW_KEY_S); }
bool Context::D_Pressed() { return glfwGetKey(window, GLFW_KEY_D); }
bool Context::A_Pressed() { return glfwGetKey(window, GLFW_KEY_A); }
bool Context::E_Pressed() { return glfwGetKey(window, GLFW_KEY_E); }
bool Context::Q_Pressed() { return glfwGetKey(window, GLFW_KEY_Q); }
bool Context::Space_Pressed() { return glfwGetKey(window, GLFW_KEY_SPACE); }
bool Context::LCtrl_Pressed() { return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL); }