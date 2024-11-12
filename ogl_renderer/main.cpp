#include "networking/Networking.h"

#include "engine/Context.h"
#include "engine/Primitives.h"
#include "engine/GameObject.h"
#include "engine/Scene.h"
#include "renderer/MeshImporter.h"
#include "renderer/Camera.h"
#include "engine/Diagnostics.h"

#include "renderer/Renderer.h"
#include "physics/Physics.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>
#include <string>

using glm::mat4;
using glm::vec3;
using namespace std;
#define TESTGRAVITYSCALE 1

int main(){

	Log("Start Server ?", FOREGROUND_BLUE);
	char responce;
	cin >> responce;
	if (responce == 'y' || responce == 'Y') {
		Networking::SetupServer();
	}
	else { Networking::SetupClient(); }
	//Window init
	Context* context = new Context();
	context->Initialize();
	Diagnostics::LogHardware();
	// Shaders / Textures / Materials Setup
	Mesh* sphereMesh = Primitives::MakeSphere();
	Mesh* planeMesh = Primitives::MakePlane();
	Shader* mainShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Shader* mainShader2 = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture* mainTexture = LoadTexture("textures/marble.png");
	Texture* mainTexture2 = LoadTexture("textures/metal.png");
	Material* mainMaterial = new Material(mainShader);
	Material* mainMaterial2 = new Material(mainShader2);
	mainMaterial->SetTexture(mainTexture);
	mainMaterial2->SetTexture(mainTexture2);
	// Object Setup
	GameObject* obj0 = new GameObject(sphereMesh, mainMaterial);
	GameObject* planeObj = new GameObject(planeMesh, mainMaterial2);
	//Make our camera
	Camera* mainCamera = new Camera;
	//Move stuff around
	mainCamera->cameraTransform.TranslateLocal(vec3(0, 3, -9));
	obj0->transform->TranslateLocal(vec3(0,5,0));
	//obj0.transform.Rotate(vec3(1,0,0),35);
	planeObj->transform->TranslateLocal(vec3(0,-5,0));
	planeObj->transform->ChangeScale(vec3(500, 1, 500));
	//Light
	Light testDirLight(vec3(1, 1, 1) / 2.f, vec3(-0.5f, 1, -0.5f));
	//Physics Setup
	Physics* phys = new Physics();
	//Each phys object gets a collider, rigidbody, and a colRbPair containing info on both
	Collider* col0 = new Collider(ColliderShapes::Sphere);
	Rigidbody* rb0 = new Rigidbody(obj0->transform, vec3(1, 0, 1), vec3(360,0,0), TESTGRAVITYSCALE, false, 1.f);
	ColRbPair* crp0 = new ColRbPair(col0, rb0);
	phys->AddColRbPair(crp0);
	Collider* planeCol = new Collider(ColliderShapes::Plane);
	Rigidbody* planeRb = new Rigidbody(planeObj->transform, vec3(0, 0, 0), 0.f, true, 1.f);
	ColRbPair* planeCrp = new ColRbPair(planeCol, planeRb);
	phys->AddColRbPair(planeCrp);
	//Scene setup
	Scene* scene = new Scene();
	scene->AddToScene(obj0);
	scene->AddToScene(planeObj);
	scene->AddToScene(mainCamera);
	scene->AddToScene(&testDirLight);
	//Time setup
	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 200;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;
	//Testing stuff
	float moveSpeed = 75.f;
	float spinSpeed = 0;
	float sensitivity = 100.f;
	float lastSpawnTime = 0;
	bool pause = false;


	
	//Main Loop
	while (!context->ShouldClose()) {
		//Tick context
		context->Tick();
		context->ClearScreen();

		if (context->Alpha1_Pressed()) { pause = !pause; }
		if (pause) { continue; }
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
		

		vec2 mouseDelta = context->GetMouseDelta();
		//Inputs
		if (context->Mouse1_Pressed() && mouseDelta.x!=0)		{ mainCamera->cameraTransform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTime); }
		if (context->Mouse1_Pressed() && mouseDelta.y!=0)		{ mainCamera->cameraTransform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTime); }

		if (context->W_Pressed())		{ mainCamera->cameraTransform.TranslateLocal(vec3(0, 0, 1) * deltaTime * moveSpeed); }
		else if (context->S_Pressed())	{ mainCamera->cameraTransform.TranslateLocal(vec3(0, 0, -1) * deltaTime * moveSpeed); }
		if (context->D_Pressed())		{ mainCamera->cameraTransform.TranslateLocal(vec3(-1, 0, 0) * deltaTime * moveSpeed); }
		else if (context->A_Pressed())	{ mainCamera->cameraTransform.TranslateLocal(vec3(1, 0, 0) * deltaTime * moveSpeed); }
		if (context->E_Pressed()) { mainCamera->cameraTransform.TranslateLocal(vec3(0, 1, 0) * deltaTime * moveSpeed); }
		else if (context->Q_Pressed()) { mainCamera->cameraTransform.TranslateLocal(vec3(0, -1, 0) * deltaTime * moveSpeed); }

		if (context->UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context->DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context->RightArrow_Pressed()) { spinSpeed += 5.f; }
		else if (context->LeftArrow_Pressed()) { spinSpeed -= 5.f; }

		if (context->Space_Pressed() && (currentTime > lastSpawnTime + 0.1f)) {
			lastSpawnTime = currentTime;

			GameObject* newObj = new GameObject(sphereMesh, mainMaterial );
			newObj->transform->TranslateGlobal(mainCamera->cameraTransform.GetPosition());
			//newObj->transform->ChangeScale(vec3(0.1f, 0.1f, 0.1f));
			Collider* newCol = new Collider(ColliderShapes::Sphere);
			Rigidbody* newRb = new Rigidbody(newObj->transform, mainCamera->cameraTransform.Forward()*75.f, vec3(0,0,0), TESTGRAVITYSCALE, false, 1.f);
			ColRbPair* newCrp = new ColRbPair(newCol, newRb);
			phys->AddColRbPair(newCrp);
			scene->AddToScene(newObj);
		}
		Networking::NetworkingTick();
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