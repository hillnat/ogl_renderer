#include "diagnostics.h"
#include "context.h"
#include "primitives.h"
#include "gameObject.h"
#include "scene.h"
#include "meshImporter.h"
#include "camera.h"
#include "physics.h"
#include "renderer.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
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
#pragma region Setup
#pragma region Context
	context context;
	context.Initialize();
#pragma endregion
#pragma region Object Setup
	gameObject obj1("obj1", MakeSphere());
	gameObject obj2("obj2", MakePlane());
	camera testCamera{};

	obj2.transform.Translate(vec3(-2, 0, 0));
	obj1.transform.Translate(vec3(2, 0, 0));
	obj2.transform.Rotate(vec3(1, 0, 0), -105);

	testCamera.transform.Translate(vec3(0, 1, -5));

	light testDirLight(vec3(1, 1, 1), vec3(0, 1, 0));
#pragma endregion
#pragma region Shaders / Textures
	shader basicshader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	texture basictexture = LoadTexture("textures/noise.png");
	SetUniform(basicshader, 3, basictexture, 0);
#pragma endregion
#pragma region Physics
	physics phys;
	phys.CreateRigidbody(&obj1.transform);
#pragma endregion
#pragma region Scene
	scene scene;
	scene.AddToScene(&obj1);
	scene.AddToScene(&obj2);
	scene.AddToScene(&testCamera);
	scene.AddToScene(&testDirLight);
#pragma endregion
#pragma region Time
	double lastTime = 0;
	double fixedDeltaTimeAccum = 0;
	const double fixedDeltaTimeThreshold = 0.01;
	const double fixedDeltaTime = (double)1 / fixedDeltaTimeThreshold;
#pragma endregion
	//Test
	gameObject* testObject = &obj1;
	float moveSpeed = 15.f;
	float sensitivity = 30.f;
	Diagnostics::LogHardware();
#pragma endregion
	//Main Loop
	while (!context.ShouldClose()) {
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		//while (fixedDeltaTimeAccum > fixedDeltaTimeThreshold) { fixedDeltaTimeAccum -= fixedDeltaTimeThreshold; phys.UpdateAllBodies(); }//Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
		float deltaTimeF = (float)deltaTime;
		lastTime = currentTime;
		context.Tick();
		context.Clear();
		vec3 wierdAxis = vec3(0.7f, 0.7f, 0.7f);                          
		//if (context.UpArrow_Pressed()) { mainCamera.transform.Rotate(vec3(1, 0, 0), 5.f * deltaTimeF); }
		//else if (context.DownArrow_Pressed()) { mainCamera.transform.Rotate(vec3(-1, 0, 0), 5.f * deltaTimeF); }
		//if (context.RightArrow_Pressed()) { mainCamera.transform.Rotate(vec3(0, -1, 0), 5.f * deltaTimeF); }
		//else if (context.LeftArrow_Pressed()) { mainCamera.transform.Rotate(vec3(0, -1, 0), 5.f * deltaTimeF); }


		vec2 mouseDelta = context.GetMouseDelta();

		if (context.Mouse1_Pressed() && mouseDelta.x!=0)		{ testCamera.transform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTimeF); }
		if (context.Mouse1_Pressed() && mouseDelta.y!=0)		{ testCamera.transform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTimeF); }

		if (context.W_Pressed())		{ testCamera.transform.Translate(vec3(0, 0, 1) * deltaTimeF * moveSpeed); }
		else if (context.S_Pressed())	{ testCamera.transform.Translate(vec3(0, 0, -1) * deltaTimeF * moveSpeed); }
		if (context.D_Pressed())		{ testCamera.transform.Translate(vec3(-1, 0, 0) * deltaTimeF * moveSpeed); }
		else if (context.A_Pressed())	{ testCamera.transform.Translate(vec3(1, 0, 0) * deltaTimeF * moveSpeed); }
		if (context.Space_Pressed()) { testCamera.transform.LookAt(testObject->transform.GetPosition()); }
		if (context.UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context.DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context.RightArrow_Pressed()) { moveSpeed += 5.f; }
		else if (context.LeftArrow_Pressed()) { moveSpeed -= 5.f; }
		if(phys.SphereSphereOverlap(testObject->transform.GetPosition(), 0.5f, testCamera.transform.GetPosition(), 0.5f)){
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