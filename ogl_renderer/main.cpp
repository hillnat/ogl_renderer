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

int main(){
	Context* context = new Context();
	context->Initialize();

// Shaders / Textures / Materials Setup
	Shader diffuseShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture treeTexture = LoadTexture("textures/treetex.png");
	Material diffuseMaterial = Material(&diffuseShader);
	SetUniform(diffuseShader, 3, treeTexture, 0);//Attach texture to shader
// Object Setup
	//GameObject obj1(Primitives::MakeSphere(), &diffuseMaterial);
	GameObject obj1("meshes/tree.obj", &diffuseMaterial);
	//GameObject obj3(Primitives::MakeSphere(), &diffuseMaterial2);
	//GameObject obj2(Primitives::MakePlane(), &basicMaterial);

	Camera testCamera{};

	testCamera.cameraTransform.TranslateLocal(vec3(0, 0, -6));
	//obj2.transform.TranslateLocal(vec3(0,-4,0));
	//obj1.transform.TranslateLocal(vec3(0,2,0));
	//obj3.transform.TranslateLocal(vec3(0,4,0));

	Light testDirLight(vec3(1, 1, 1) / 2.f, vec3(-0.5f, 1, -0.5f));
//Physics Setup
	Physics* phys = new Physics();
	//Collider col1 = Collider(ColliderShapes::Sphere);
	//Rigidbody rb1 = Rigidbody(&obj1.transform, vec3(0, 0, 0), false, false, 1.f);
	//ColRbPair crp1 = ColRbPair(&col1, &rb1);
	//phys->AddColRbPair(&crp1);
	//Collider col2 = Collider(ColliderShapes::Plane);
	//Rigidbody rb2 = Rigidbody(&obj2.transform, vec3(0, 0, 0), false, false, 1.f);
	//ColRbPair crp2 = ColRbPair(&col2, &rb2);
	//phys->AddColRbPair(&crp2);
	//Collider col3 = Collider(ColliderShapes::Sphere);
	//Rigidbody rb3 = Rigidbody(&obj3.transform, vec3(0, -0.7f, 0), false, false, 3.f);
	//ColRbPair crp3 = ColRbPair(&col3, &rb3);
	//phys->AddColRbPair(&crp3);

	Scene* scene = new Scene();
	scene->AddToScene(&obj1);
	//scene->AddToScene(&obj2);
	//scene->AddToScene(&obj3);
	scene->AddToScene(&testCamera);
	scene->AddToScene(&testDirLight);

	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 100;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;

	//Test
	float moveSpeed = 15.f;
	float spinSpeed = 0;
	float sensitivity = 30.f;
	Diagnostics::LogHardware();
	//Main Loop
	while (!context->ShouldClose()) {
		//Get delta times
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		lastTime = currentTime;
		//Physics Timestep
		while (fixedDeltaTimeAccum > fixedDeltaTime) { 
			fixedDeltaTimeAccum -= fixedDeltaTime;
			phys->UpdateAllBodies(fixedDeltaTime);
		}//Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
		//Tick context
		context->Tick();
		context->ClearScreen();

		//Rotate random stuff for testing
		obj1.transform.Rotate(vec3(0,0,1), spinSpeed * deltaTime);
		vec2 mouseDelta = context->GetMouseDelta();

		if (context->Mouse1_Pressed() && mouseDelta.x!=0)		{ testCamera.cameraTransform.Rotate(vec3(0, 1, 0), -mouseDelta.x * sensitivity * deltaTime); }
		if (context->Mouse1_Pressed() && mouseDelta.y!=0)		{ testCamera.cameraTransform.Rotate(vec3(1, 0, 0), mouseDelta.y * sensitivity * deltaTime); }

		if (context->W_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, 1) * deltaTime * moveSpeed); }
		else if (context->S_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(0, 0, -1) * deltaTime * moveSpeed); }
		if (context->D_Pressed())		{ testCamera.cameraTransform.TranslateLocal(vec3(-1, 0, 0) * deltaTime * moveSpeed); }
		else if (context->A_Pressed())	{ testCamera.cameraTransform.TranslateLocal(vec3(1, 0, 0) * deltaTime * moveSpeed); }

		if (context->UpArrow_Pressed()) { sensitivity += 10.f; }
		else if (context->DownArrow_Pressed()) { sensitivity -= 10.f; }
		if (context->RightArrow_Pressed()) { spinSpeed += 5.f; }
		else if (context->LeftArrow_Pressed()) { spinSpeed -= 5.f; }

		scene->RenderAll();
		Diagnostics::CheckGLError();
	}
	//Program ending
	scene->FreeAllMeshes();
	delete(scene);
	delete(phys);
	//Refactor freeing of mats and textures
	//FreeTexture(basictexture);
	context->Terminate();
	delete(context);

	return 0;
}