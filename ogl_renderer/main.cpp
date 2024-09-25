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
	GameObject obj1("Player", "meshes/Pointer.obj");
	obj1.transform.Translate(vec3(0, 0, -10.f));
	Camera mainCamera{};

	scene.AddToScene(&obj1);
	scene.AddToScene(&mainCamera);
	vec3 colors[]{ vec3(1,0,0),vec3(0,1,0) ,vec3(0,0,1) };
	vec3 dirs[]{ vec3(1,0,0),vec3(0,1,0) ,vec3(0,0,1) };
	Light directionalLight1(colors[0], dirs[0]);
	Light directionalLight2(colors[1], dirs[1]);
	Light directionalLight3(colors[2], dirs[2]);
	scene.AddToScene(&directionalLight1);
	scene.AddToScene(&directionalLight2);
	scene.AddToScene(&directionalLight3);
	

	//Shaders
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/basic.frag");
	//Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/tex.PNG");
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float SPEED = 20.f;
	const float MOUSESPEED = 1.5f;
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

		if (context.D_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * SPEED * deltaTimeF); }
		else if (context.A_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * -SPEED * deltaTimeF); }
		if (context.W_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * SPEED * deltaTimeF); }
		else if (context.S_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * -SPEED * deltaTimeF); }
		if (context.Space_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * SPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * -SPEED * deltaTimeF); }
		if (context.E_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), MOUSESPEED * deltaTimeF); }
		else if (context.Q_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), -MOUSESPEED * deltaTimeF); }

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