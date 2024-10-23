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
void CheckGLError()
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
	Context context;
	context.Initialize();
#pragma endregion

#pragma region Shaders / Textures / Materials
	Shader basicShader1 = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	//Shader basicShader2 = LoadShader("shaders/basic.vert", "shaders/basic.frag");
	//Shader lineShader = LoadShader("shaders/basic.vert", "shaders/lines.frag");
	//Texture basictexture = LoadTexture("textures/noise.png");
	Texture karambitTexture = LoadTexture("textures/karambitUV.png");
	SetUniform(basicShader1, 3, karambitTexture, 0);
	Material basicMaterial1 = Material(&basicShader1);
	//Material lineMaterial = Material(&lineShader);
#pragma endregion
#pragma region Object Setup
	GameObject karambitObj("obj1", "meshes/karambit.obj", &basicMaterial1);


	Camera testCamera{};

	testCamera.cameraTransform.TranslateLocal(vec3(0, 0.25f, -3));

	Light testDirLight(vec3(1, 1, 1)/2.f, vec3(-0.5f, -1, -0.5f));
#pragma endregion
#pragma region Physics
	Physics phys;
	//Collider col1 = Collider(ColliderShapes::Sphere);
	//Rigidbody rb1 = Rigidbody(&sphere1.transform, vec3(0, 0, 0), true, false, 1.f);
	//ColRbPair crp1 = ColRbPair(&col1, &rb1);
	//phys.AddColRbPair(&crp1);
	karambitObj.transform.Rotate(vec3(1, 0, 0), 85);

#pragma endregion
#pragma region Scene
	Scene scene;
	scene.AddToScene(&karambitObj);
	scene.AddToScene(&testCamera);
	scene.AddToScene(&testDirLight);
#pragma endregion
#pragma region Time
	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 100;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;
#pragma endregion
	//Test
	float moveSpeed = 15.f;
	float spinSpeed = 0;
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
		karambitObj.transform.Rotate(vec3(0,0,1), spinSpeed * deltaTime);
		vec2 mouseDelta = context.GetMouseDelta();

		if (context.Mouse1_Pressed() && mouseDelta.x!=0)		{ testCamera.cameraTransform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTime); }
		if (context.Mouse1_Pressed() && mouseDelta.y!=0)		{ testCamera.cameraTransform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTime); }

		if (context.W_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, 1) * deltaTime * moveSpeed); }
		else if (context.S_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, -1) * deltaTime * moveSpeed); }
		if (context.D_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(-1, 0, 0) * deltaTime * moveSpeed); }
		else if (context.A_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(1, 0, 0) * deltaTime * moveSpeed); }


		if (context.UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context.DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context.RightArrow_Pressed()) { spinSpeed += 5.f; }
		else if (context.LeftArrow_Pressed()) { spinSpeed -= 5.f; }

		scene.RenderAll();
		CheckGLError();
	}
	//Program ending
	scene.FreeAllMeshes();
	//Refactor freeing of mats and textures
	//FreeTexture(basictexture);
	context.Terminate();
	return 0;
}