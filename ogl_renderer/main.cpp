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
int main()
{
	Context context;
	context.Initialize();
	Diagnostics::Environment();
//Scene Definitions
	Scene scene;
	GameObject obj1("Player", "meshes/fish2.obj");
	obj1.transform.Translate(vec3(0, 0, -5.f));
	Camera mainCamera{};
	Light directionalLight{vec3(1.f), vec3(1.f)};
	scene.AddToScene(&obj1);
	scene.AddToScene(&mainCamera);
	scene.AddToScene(&directionalLight);
	//Shaders
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/tex.PNG");
//
	
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float SPEED = 20;
	const float MOUSESPEED = 200;
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
		else if (context.A_Pressed()) { myPlayer->transform.Translate(-myPlayer->transform.Right() * SPEED * deltaTimeF); }
		if (context.W_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * SPEED * deltaTimeF); }
		else if (context.S_Pressed()) { myPlayer->transform.Translate(-myPlayer->transform.Forward() * SPEED * deltaTimeF); }
		if (context.Space_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * SPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { myPlayer->transform.Translate(-myPlayer->transform.Up() * SPEED * deltaTimeF); }
		if (context.E_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), MOUSESPEED * deltaTimeF); }
		else if (context.Q_Pressed()) { myPlayer->transform.Rotate(myPlayer->transform.Up(), -MOUSESPEED * deltaTimeF); }

		scene.DrawAll(&basicShader);
	}
	FreeMesh(obj1.mesh);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.Terminate();
	return 0;
}