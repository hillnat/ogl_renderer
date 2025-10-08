#include "engine/Context.h"
#include "renderer/Renderer.h"
#include "engine/Primitives.h"
#include "engine/GameObject.h"
#include "engine/Scene.h"
#include "renderer/Material.h"
#include "engine/Diagnostics.h"
#include "glm/ext.hpp" //GLM provides us with matrices and math functions
#include "glfw/glfw3.h"	//GLFW is a Windows API wrapper, allows us to handle window context easily
#include <iostream>
#include <string>

using glm::mat4;
using glm::vec3;
using namespace std;
using namespace MeshImporter;
#define TESTGRAVITYSCALE 1

const char* path = "meshes/chogath.glb";

int main(){
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	//Window init
	Context* context = new Context();
	context->Initialize();
	Diagnostics::LogHardware();

	Mesh* characterMesh = MeshImporter::ImportFromFileGlb(path);
	Mesh* planeMesh = Primitives::MakePlane();

	
	Shader* characterShader = Renderer::LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	//Shader* planeShader = Renderer::LoadShader("shaders/basic.vert", "shaders/basic.frag");



	

	std::vector<Texture*> characterTextures = MeshImporter::ExtractTexturesFromFileGlb(path);
	Texture* planeTexture = MeshImporter::LoadTexture("textures/noise.png");
	Material* planeMaterial = new Material(characterShader);
	Material* characterMaterial = new Material(characterShader);
	characterMaterial->SetTexture(characterTextures);
	planeMaterial->SetTexture(planeTexture);
	GameObject* characterObj = new GameObject(characterMesh, characterMaterial);
	GameObject* planeObj = new GameObject(planeMesh, planeMaterial);
	Camera* mainCamera = new Camera;
	characterObj->material = characterMaterial;
	planeObj->material = planeMaterial;
	mainCamera->cameraTransform.TranslateLocal(vec3(0, 1, -2.5));
	planeObj->transform->SetRotation(vec3(0,0,1),180.f);
	characterObj->transform->ChangeScale(vec3(0.01f, 0.01f, 0.01f));
	planeObj->transform->ChangeScale(vec3(200.f, 1.f, 200.f));
	//Light
	Light testDirLight(vec3(1, 1, 1), vec3(1, 1, -0.5f));
	characterObj->transform->Rotate(glm::vec3(0.f, 1.0f, 0.f), 180.f);

	//Scene setup
	Scene* scene = new Scene();
	scene->AddToScene(characterObj);
	scene->AddToScene(planeObj);
	scene->AddToScene(mainCamera);
	scene->AddToScene(&testDirLight);
	//Time setup
	float lastTime = glfwGetTime();
	float fixedDeltaTimeAccum = 0;
	const float fixedTimeStepsPerSec = 200;
	const float fixedDeltaTime = 1.f / fixedTimeStepsPerSec;
	float lastChangeAnimTime = 0;


	
	while (!context->ShouldClose() && !context->Space_Pressed()) {
		//Tick context
		context->Tick();
		context->ClearScreen();

		//Get delta times
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		fixedDeltaTimeAccum += deltaTime;
		lastTime = currentTime;
		//Physics Timestep, Run in a WHILE loop, to allow multiple fixed time steps to happen in the same frame given the delta time is larger than the threshold*2
		while (fixedDeltaTimeAccum > fixedDeltaTime) {
			fixedDeltaTimeAccum -= fixedDeltaTime;
			if (context->RightArrow_Pressed()) {
				characterObj->transform->Rotate(glm::vec3(0.f, 1.0f, 0.f), 0.5f);
			}
			else if (context->LeftArrow_Pressed()) {
				characterObj->transform->Rotate(glm::vec3(0.f, 1.0f, 0.f), -0.5f);
			}
		}
		if (currentTime > lastChangeAnimTime + 0.1f) {
			
			if (context->UpArrow_Pressed()) { MeshImporter::animationIndex++; lastChangeAnimTime = currentTime; }
			else if (context->DownArrow_Pressed()) { MeshImporter::animationIndex--;lastChangeAnimTime = currentTime; }
		}
		
		//Draw and error check
		Renderer::DrawObjects(scene);
		Diagnostics::CheckGLError();
	}
	//Program ending
	scene->FreeAllMeshes();
	delete(scene);
	delete(mainCamera);
	//Refactor freeing of mats and textures
	for (int i = characterTextures.size()-1;i >= 0;i--) {

		MeshImporter::FreeTexture(characterTextures[i]);
	}
	context->Terminate();
	delete(context);

	return 0;
}