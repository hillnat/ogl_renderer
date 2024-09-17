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
	context.init(640, 480, "Window");
	Diagnostics::Environment();
//Scene Definitions
	Scene scene;
	GameObject myGO("GO1", "meshes/fish2.obj");
	Transform myPlayer;
	Camera mainCamera(&myPlayer);
	Light directionalLight{vec3(1.f), vec3(1.f)};
	scene.AddToScene(&myGO);
	scene.AddToScene(&mainCamera);
	scene.AddToScene(&directionalLight);
	//Shaders
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/tex.PNG");
//
	
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float SPEED = 50;
	double lastTime = 0;
	SetUniform(basicShader, 0, mainCamera.ProjectionMatrix);
	SetUniform(basicShader, 3, basicTexture, 0);
	SetUniform(basicShader, 5, directionalLight.color);
	SetUniform(basicShader, 6, directionalLight.direction);
	while (!context.shouldClose()) {
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastTime;
		float deltaTimeF = (float)deltaTime;
		lastTime = nowTime;
		context.tick();
		context.clear();

		//myGO.transform.Rotate(vec3{ 0.f,0.01f,0.f });
        if (context.A_Pressed()) { mainCamera.transform.LocalParentTranslate(vec3(-1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.D_Pressed()) { mainCamera.transform.LocalParentTranslate( vec3(1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		if (context.W_Pressed()) { mainCamera.transform.LocalParentTranslate( vec3(0.f, 0.f, -1.f) * SPEED * deltaTimeF); }
		else if (context.S_Pressed()) { mainCamera.transform.LocalParentTranslate( vec3(0.f, 0.f, 1.f) * SPEED * deltaTimeF); }
		if (context.Space_Pressed()) { mainCamera.transform.LocalParentTranslate( vec3(0.f, 1.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { mainCamera.transform.LocalParentTranslate(vec3(0.f, -1.f, 0.f) * SPEED * deltaTimeF); }

		vec2 mouseDelta = context.GetMouseDelta();
		mouseDelta *= deltaTime;
		mouseDelta *= 50;
		if (glm::abs(mouseDelta.x) != 0.f || glm::abs(mouseDelta.y) != 0.f) {
			myPlayer.RotateEuler(myPlayer.up(), mouseDelta.x);
			mainCamera.transform.RotateEuler(myPlayer.right(), -mouseDelta.y);
		}
		//std::cout <<"Right : " << myPlayer.right().x << " | " << myPlayer.right().y << " | " << myPlayer.right().z << std::endl;
		std::cout <<"Euler : " << myPlayer.eulerAngles().x << " | " << myPlayer.eulerAngles().y << " | " << myPlayer.eulerAngles().z << std::endl;
		//std::cout <<"Rotation : " << mainCamera.transform.rotation.x << " | " << mainCamera.transform.rotation.y << " | " << mainCamera.transform.rotation.z << " | " << mainCamera.transform.rotation.w << std::endl;
		//std::cout <<"Rotation (Inverse) : " << glm::inverse(mainCamera.transform.rotation).x << " | " << glm::inverse(mainCamera.transform.rotation).y << " | " << glm::inverse(mainCamera.transform.rotation).z << " | " << glm::inverse(mainCamera.transform.rotation).w << std::endl;

		//mainCamera.transform.RotateQuat(-mainCamera.transform.rotation);
		scene.DrawAll(&basicShader);
	}
	FreeMesh(myGO.mesh);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.terminate();
	return 0;
}