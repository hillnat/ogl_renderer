#include "diagnostics.h"
#include "context.h"
#include "primitives.h"
#include "gameObject.h"
#include "scene.h"
#include "meshImporter.h"
#include "camera.h"
#include "physics.h"
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
	//Setup
	Context context;
	context.Initialize();
	Diagnostics::Environment();
//Scene Definitions
	scene scene;
	physics phys;
	gameObject obj1("Player", "meshes/axisPointer.obj");
	//GameObject obj1("Player", MakePlane());
	obj1.transform.Translate(vec3(0, 0, 5));
	phys.CreateRigidbody(&obj1.transform);
	camera mainCamera{};
	//mainCamera.transform.Rotate(vec3(1, 0, 0), 90.f);
	scene.AddToScene(&obj1);
	scene.AddToScene(&mainCamera);
	light directionallight1(vec3(1,1,1), vec3(0, -1, 0));
	scene.AddToScene(&directionallight1);

	//shaders
	shader basicshader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	texture basictexture = LoadTexture("textures/noise.png");
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float MOVESPEED = 5.f;
	const float TURNSPEED = 80.f;
	const float lightTURNSPEED = 80.f;
	SetUniform(basicshader, 3, basictexture, 0);
	//Test
	gameObject* myPlayer = &obj1;

#pragma region Time
	double lastTime = 0;
	double fixedDeltaTimeAccum = 0;
	const double fixedDeltaTimeThreshold = 0.05;
	const double fixedDeltaTime = (double)1 / fixedDeltaTimeThreshold;
#pragma endregion



	//Main Loop
	while (!context.ShouldClose()) {
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		while (fixedDeltaTimeAccum > fixedDeltaTimeThreshold) { fixedDeltaTimeAccum -= fixedDeltaTimeThreshold; phys.UpdateAllBodies(); }//Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
		float deltaTimeF = (float)deltaTime;
		lastTime = currentTime;
		context.Tick();
		context.Clear();
		vec3 wierdAxis = vec3(0.7f, 0.7f, 0.7f);
		if (context.UpArrow_Pressed()) { mainCamera.transform.Rotate(vec3(1, 0, 0), 5.f * deltaTimeF); }
		else if (context.DownArrow_Pressed()) { mainCamera.transform.Rotate(vec3(-1, 0, 0), 5.f * deltaTimeF); }
		if (context.RightArrow_Pressed()) { mainCamera.transform.Rotate(vec3(0, -1, 0), 5.f * deltaTimeF); }
		else if (context.LeftArrow_Pressed()) { mainCamera.transform.Rotate(vec3(0, -1, 0), 5.f * deltaTimeF); }

		if (context.E_Pressed()) { myPlayer->transform.Rotate(wierdAxis, -TURNSPEED * deltaTimeF); }
		else if (context.Q_Pressed()) { myPlayer->transform.Rotate(wierdAxis, TURNSPEED * deltaTimeF); }
		if (context.D_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * MOVESPEED * deltaTimeF); }
		else if (context.A_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Right() * -1.f * MOVESPEED * deltaTimeF); }
		if (context.W_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * MOVESPEED * deltaTimeF); }
		else if (context.S_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Forward() * -1.f * MOVESPEED * deltaTimeF); }
		if (context.Space_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * MOVESPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { myPlayer->transform.Translate(myPlayer->transform.Up() * -1.f * MOVESPEED * deltaTimeF); }

		if(phys.SphereSphereOverlap(myPlayer->transform.GetPosition(), 0.5f, mainCamera.transform.GetPosition(), 0.5f)){
			std::cout << "Hit!" << std::endl;
		}

		scene.RenderAll(&basicshader);
		checkGLError();
	}
	//Program ending
	FreeMesh(obj1.mesh);
	FreeShader(basicshader);
	FreeTexture(basictexture);
	context.Terminate();
	return 0;
}