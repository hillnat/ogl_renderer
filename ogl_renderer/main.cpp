#include "diagnostics.h"
#include "context.h"
#include "primitives.h"
#include "renderer.h"
#include "glm/ext.hpp"
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include "meshImporter.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>
#include <string>
using glm::mat4;
using glm::vec3;

int main()
{
	Context context;
	MeshImporter meshImporter;
	context.init(640, 480, "Window");
	Diagnostics::Environment();
	
	//Geometry myGeo = MakeCube2();//Make cube verticies
	Geometry myGeo= meshImporter.ImportFromFile("meshes/thing.obj");
	
	mat4 myGeoTransform = glm::identity<mat4>();//Transform for cube
	mat4 cameraTransform = glm::identity<mat4>();

	mat4 proj = glm::perspective(glm::radians(100.f), 640.f / 480.f, 0.01f, 5000.f);
	Shader basicShader = LoadShader("shaders/basic.vert","shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/tex.PNG");
	Light directionalLight;
	directionalLight.color = WHITE;
	directionalLight.direction = vec3{ 1.f,0.f,1.f };
	cameraTransform = glm::translate(cameraTransform, vec3{ 0,0,5.f });
	const float SPEED = 500;
	double lastTime = 0;
	SetUniform(basicShader, 0, proj);
	SetUniform(basicShader, 3, basicTexture, 0);
	SetUniform(basicShader, 5, directionalLight.color);
	SetUniform(basicShader, 6, directionalLight.direction);
	while (!context.shouldClose()) {
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastTime;
		float deltaTimeF = (float)deltaTime;
		lastTime = nowTime;
		std::cout << "FPS : " << glm::iround(1.0f / deltaTimeF) << std::endl;
		context.tick();
		context.clear();
		cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0

		myGeoTransform = glm::rotate(myGeoTransform, 0.03f, vec3{ 0.f,1.f,0.f });
        if (context.A_Pressed()) { cameraTransform =			glm::translate(cameraTransform, vec3(-1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.D_Pressed()) { cameraTransform =		glm::translate(cameraTransform, vec3(1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		if (context.W_Pressed()) { cameraTransform =			glm::translate(cameraTransform, vec3(0.f, 0.f, -1.f) * SPEED * deltaTimeF); }
		else if (context.S_Pressed()) { cameraTransform =		glm::translate(cameraTransform, vec3(0.f, 0.f, 1.f) * SPEED * deltaTimeF); }
		if (context.Space_Pressed()) { cameraTransform =		glm::translate(cameraTransform, vec3(0.f, -1.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { cameraTransform =	glm::translate(cameraTransform, vec3(0.f, 1.f, 0.f) * SPEED * deltaTimeF); }
		if (context.E_Pressed()) { cameraTransform =			glm::rotate(cameraTransform, 100.f * deltaTimeF, vec3{ 0.f,1.f,0.f }); }
		else if (context.Q_Pressed()) { cameraTransform =		glm::rotate(cameraTransform, -100.f* deltaTimeF, vec3{ 0.f,1.f,0.f }); }
		SetUniform(basicShader, 1, cameraTransform);//Camera transform
		SetUniform(basicShader, 2, myGeoTransform);//Cube transform
		SetUniform(basicShader, 7, vec3(cameraTransform[3]));//Cam position for specular. Mat4 to vec3 conversion
		DrawGeometry(basicShader, myGeo);
	}
	FreeGeometry(myGeo);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.terminate();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
