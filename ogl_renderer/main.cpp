#include "diagnostics.h"
#include "context.h"
#include "primitives.h"
#include "gameObject.h"
#include "scene.h"
#include "meshImporter.h"
#include "camera.h"
#include "renderer.h"
#include "glm/ext.hpp"
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>
#include <string>
using glm::mat4;
using glm::vec3;
void checkGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}
}
int main()
{
	Context context;
	context.Initialize();
	Diagnostics::Environment();
//Scene Definitions
	Scene scene;
	GameObject obj1("Player", "meshes/axisPointer.obj");
	obj1.transform.Translate(vec3(0, -3, 0));
	Camera mainCamera{};
	mainCamera.transform.Rotate(vec3(1, 0, 0), 90);
	scene.AddToScene(&obj1);
	scene.AddToScene(&mainCamera);
	Light directionalLight1(vec3(0.8F,0.8F,0.8F), vec3(0, -1, 0));
	scene.AddToScene(&directionalLight1);

	//Shaders
	//Shader basicShader = LoadShader("shaders/basic.vert", "shaders/basic.frag");
	//Shader lineShader = LoadShader("shaders/lines.vert", "shaders/lines.frag");
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/axisPointer_UV.png");
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float MOVESPEED = 5.f;
	const float TURNSPEED = 40.f;
	const float CAMTURNSPEED = 40.f;
	double lastTime = 0;
	SetUniform(basicShader, 3, basicTexture, 0);
	//Test
	GameObject* myPlayer = &obj1;
	while (!context.ShouldClose()) {
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastTime;
		float deltaTimeF = (float)deltaTime;
		lastTime = nowTime;
		context.Tick();
		context.Clear();

		if (context.UpArrow_Pressed()) { mainCamera.transform.Rotate(mainCamera.transform.Right(), CAMTURNSPEED * deltaTimeF); }
		else if (context.DownArrow_Pressed()) { mainCamera.transform.Rotate(mainCamera.transform.Right(), -CAMTURNSPEED * deltaTimeF); }
		if (context.RightArrow_Pressed()) { mainCamera.transform.Rotate(mainCamera.transform.Up(), -CAMTURNSPEED * deltaTimeF); }
		else if (context.LeftArrow_Pressed()) { mainCamera.transform.Rotate(mainCamera.transform.Up(), CAMTURNSPEED * deltaTimeF); }         
		if (context.E_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), -TURNSPEED * deltaTimeF); }
		else if (context.Q_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), TURNSPEED * deltaTimeF); }
		if (context.D_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * MOVESPEED * deltaTimeF); }
		else if (context.A_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * -MOVESPEED * deltaTimeF); }
		if (context.W_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * -MOVESPEED * deltaTimeF); }
		else if (context.S_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * MOVESPEED * deltaTimeF); }
		if (context.Space_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * MOVESPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * -MOVESPEED * deltaTimeF); }
		

		

		std::cout << "Forward : " << myPlayer->transform.Forward().x << " | " << myPlayer->transform.Forward().y << " | " << myPlayer->transform.Forward().z << " | " << std::endl;
		std::cout << "Up      : " << myPlayer->transform.Up().x << " | " << myPlayer->transform.Up().y << " | " << myPlayer->transform.Up().z << " | " << std::endl;
		std::cout << "Right   : " << myPlayer->transform.Right().x << " | " << myPlayer->transform.Right().y << " | " << myPlayer->transform.Right().z << " | " << std::endl;
		std::cout << "-----------------------------------------------------------------" << std::endl;
		scene.DrawAll(&basicShader);
		//checkGLError();
	}
	FreeMesh(obj1.mesh);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.Terminate();
	return 0;
}