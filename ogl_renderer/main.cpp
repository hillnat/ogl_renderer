#include "engine/Diagnostics.h"
#include "engine/Context.h"
#include "engine/Primitives.h"
#include "engine/GameObject.h"
#include "engine/Scene.h"
#include "renderer/MeshImporter.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>
#include <string>
#include "physics/Physics.h"
using glm::mat4;
using glm::vec3;

#define TESTGRAVITYSCALE 0.05

int main(){
	//Window init
	Context* context = new Context();
	context->Initialize();
	// Shaders / Textures / Materials Setup
	Mesh sphereMesh = Primitives::MakeSphere();
	Shader mainShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture mainTexture = LoadTexture("textures/marble.png");
	Material mainMaterial = Material(&mainShader);
	mainMaterial.SetTexture(&mainTexture);
	// Object Setup
	GameObject obj0(sphereMesh, &mainMaterial);
	GameObject planeObj(Primitives::MakePlane(), &mainMaterial);
	//Make our camera
	Camera* mainCamera = new Camera;
	//Move stuff around
	mainCamera->cameraTransform.TranslateLocal(vec3(0, 3, -9));
	obj0.transform.TranslateLocal(vec3(0,5,0));
	planeObj.transform.TranslateLocal(vec3(0,-5,0));
	planeObj.transform.ChangeScale(vec3(5, 1, 5));
	//Light
	Light testDirLight(vec3(1, 1, 1) / 2.f, vec3(-0.5f, 1, -0.5f));
	//Physics Setup
	Physics* phys = new Physics();
	//Each phys object gets a collider, rigidbody, and a colRbPair containing info on both
	Collider col0 = Collider(ColliderShapes::Sphere);
	Rigidbody rb0 = Rigidbody(&obj0.transform, vec3(0, 0, 0), TESTGRAVITYSCALE, false, 1.f);
	ColRbPair crp0 = ColRbPair(&col0, &rb0);
	phys->AddColRbPair(&crp0);
	Collider planeCol = Collider(ColliderShapes::Plane);
	Rigidbody planeRb = Rigidbody(&planeObj.transform, vec3(0, 0, 0), 0.f, true, 1.f);
	ColRbPair planeCrp = ColRbPair(&planeCol, &planeRb);
	phys->AddColRbPair(&planeCrp);
	//Scene setup
	Scene* scene = new Scene();
	scene->AddToScene(&obj0);
	scene->AddToScene(&planeObj);
	scene->AddToScene(mainCamera);
	scene->AddToScene(&testDirLight);
	//Time setup
	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 500;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;
	//Testing stuff
	float moveSpeed = 15.f;
	float spinSpeed = 0;
	float sensitivity = 30.f;
	float lastSpawnTime = 0;
	Diagnostics::LogHardware();
	std::cout << "PRESS SPACE TO SPAWN BALLS" << std::endl;
	//Main Loop
	while (!context->ShouldClose()) {
		//Get delta times
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		lastTime = currentTime;
		//Physics Timestep, Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
		while (fixedDeltaTimeAccum > fixedDeltaTime) { 
			fixedDeltaTimeAccum -= fixedDeltaTime;
			phys->UpdateAllBodies(fixedDeltaTime);
		}
		//Tick context
		context->Tick();
		context->ClearScreen();

		vec2 mouseDelta = context->GetMouseDelta();
		//Inputs
		if (context->Mouse1_Pressed() && mouseDelta.x!=0)		{ mainCamera->cameraTransform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTime); }
		if (context->Mouse1_Pressed() && mouseDelta.y!=0)		{ mainCamera->cameraTransform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTime); }

		if (context->W_Pressed())		{ mainCamera->cameraTransform.TranslateLocal(vec3(0, 0, 1) * deltaTime * moveSpeed); }
		else if (context->S_Pressed())	{ mainCamera->cameraTransform.TranslateLocal(vec3(0, 0, -1) * deltaTime * moveSpeed); }
		if (context->D_Pressed())		{ mainCamera->cameraTransform.TranslateLocal(vec3(-1, 0, 0) * deltaTime * moveSpeed); }
		else if (context->A_Pressed())	{ mainCamera->cameraTransform.TranslateLocal(vec3(1, 0, 0) * deltaTime * moveSpeed); }
		if (context->E_Pressed()) { mainCamera->cameraTransform.TranslateLocal(vec3(0, -1, 0) * deltaTime * moveSpeed); }
		else if (context->Q_Pressed()) { mainCamera->cameraTransform.TranslateLocal(vec3(0, 1, 0) * deltaTime * moveSpeed); }

		if (context->UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context->DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context->RightArrow_Pressed()) { spinSpeed += 5.f; }
		else if (context->LeftArrow_Pressed()) { spinSpeed -= 5.f; }

		if (context->Space_Pressed() && (currentTime > lastSpawnTime + 0.1f)) {
			lastSpawnTime = currentTime;

			GameObject* newObj = new GameObject(sphereMesh, &mainMaterial);
			scene->AddToScene(newObj);
			newObj->transform.TranslateLocal(vec3(0, 10, 0));
			Collider* newCol = new Collider(ColliderShapes::Sphere);
			Rigidbody* newRb = new Rigidbody(&newObj->transform, vec3(0, 50, 0), TESTGRAVITYSCALE, false, 1.f);
			ColRbPair* newCrp = new ColRbPair(newCol, newRb);
			phys->AddColRbPair(newCrp);
		}

		//Draw and error check
		scene->RenderAll();
		Diagnostics::CheckGLError();
	}
	//Program ending
	scene->FreeAllMeshes();
	delete(scene);
	delete(phys);
	delete(mainCamera);
	//Refactor freeing of mats and textures
	FreeTexture(mainTexture);
	context->Terminate();
	delete(context);

	return 0;
}