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

#pragma region Shaders / Textures / Materials
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Shader basicShader2 = LoadShader("shaders/basic.vert", "shaders/basic.frag");
	Texture basictexture = LoadTexture("textures/noise.png");
	SetUniform(basicShader, 3, basictexture, 0);
	Material basicMaterial = Material(&basicShader);
	Material basicMaterial2 = Material(&basicShader2);
#pragma endregion
#pragma region Object Setup
	GameObject obj1("obj1", MakeSphere(), &basicMaterial);
	GameObject obj2("obj2", MakeSphere(), &basicMaterial2);
	Camera testCamera{};

	obj2.transform.TranslateLocal(vec3(-3, 0, 0));
	obj1.transform.TranslateLocal(vec3(3, 0, 0));

	testCamera.cameraTransform.TranslateLocal(vec3(0, 1, -5));

	Light testDirLight(vec3(1, 1, 1), vec3(0, 1, 0));
#pragma endregion
#pragma region Physics
	Physics phys;
	phys.CreateRigidbody(&obj1.transform, vec3(-2,0,0), false, 1.f);
	phys.CreateRigidbody(&obj2.transform, vec3(2, 0, 0), false, 1.f);
#pragma endregion
#pragma region Scene
	Scene scene;
	scene.AddToScene(&obj1);
	scene.AddToScene(&obj2);
	scene.AddToScene(&testCamera);
	scene.AddToScene(&testDirLight);
#pragma endregion
#pragma region Time
	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 200;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;
#pragma endregion
	//Test
	GameObject* testObject = &obj1;
	float moveSpeed = 15.f;
	float sensitivity = 30.f;
	Diagnostics::LogHardware();
	std::cout << "Fixed Delta Time : " << fixedDeltaTime << std::endl;
#pragma endregion
	//Main Loop
	while (!context.ShouldClose()) {
#pragma region Handle Time
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		lastTime = currentTime;
#pragma endregion
#pragma region Handle Fixed Timestep
		while (fixedDeltaTimeAccum > fixedDeltaTime) { 
			fixedDeltaTimeAccum -= fixedDeltaTime;
			phys.UpdateAllBodies(fixedDeltaTime);
		}//Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
#pragma endregion
#pragma region Handle Context
		context.Tick();
		context.ClearScreen();
#pragma endregion
		//Rotate random stuff for testing
		obj1.transform.Rotate(vec3(1, 1, 1), 25 * deltaTime);
		obj2.transform.Rotate(vec3(1, 1, 1), 25 * deltaTime);
		
		vec2 mouseDelta = context.GetMouseDelta();

		if (context.Mouse1_Pressed() && mouseDelta.x!=0)		{ testCamera.cameraTransform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTime); }
		if (context.Mouse1_Pressed() && mouseDelta.y!=0)		{ testCamera.cameraTransform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTime); }

		if (context.W_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, 1) * deltaTime * moveSpeed); }
		else if (context.S_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, -1) * deltaTime * moveSpeed); }
		if (context.D_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(-1, 0, 0) * deltaTime * moveSpeed); }
		else if (context.A_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(1, 0, 0) * deltaTime * moveSpeed); }
		if (context.UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context.DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context.RightArrow_Pressed()) { moveSpeed += 5.f; }
		else if (context.LeftArrow_Pressed()) { moveSpeed -= 5.f; }


		scene.RenderAll();
		checkGLError();
	}
	//Program ending
	FreeMesh(obj1.mesh);
	FreeShader(basicShader);
	FreeTexture(basictexture);
	context.Terminate();
	return 0;
}