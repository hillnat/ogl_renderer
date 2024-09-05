#include "diagnostics.h"
#include "context.h"
#include "primitives.h"
#include "gameObject.h"
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
	GameObject myGO("MyGo", "meshes/car.obj");
	Camera mainCamera;
	Light directionalLight;
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/tex.PNG");
//
	
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float SPEED = 5000;
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

		myGO.transform.Rotate(0.3f * deltaTimeF, vec3{ 0.f,1.f,0.f });
        if (context.A_Pressed()) { mainCamera.transform.TranslateL(vec3(-1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.D_Pressed()) { mainCamera.transform.TranslateL( vec3(1.f, 0.f, 0.f) * SPEED * deltaTimeF); }
		if (context.W_Pressed()) { mainCamera.transform.TranslateL( vec3(0.f, 0.f, 1.f) * SPEED * deltaTimeF); }
		else if (context.S_Pressed()) { mainCamera.transform.TranslateL( vec3(0.f, 0.f, -1.f) * SPEED * deltaTimeF); }
		if (context.Space_Pressed()) { mainCamera.transform.TranslateL( vec3(0.f, -1.f, 0.f) * SPEED * deltaTimeF); }
		else if (context.LCtrl_Pressed()) { mainCamera.transform.TranslateL(vec3(0.f, 1.f, 0.f) * SPEED * deltaTimeF); }
		if (context.E_Pressed()) { mainCamera.transform.Rotate(10.f * deltaTimeF, vec3{ 0.f,1.f,0.f }); }
		else if (context.Q_Pressed()) { mainCamera.transform.Rotate(-10.f* deltaTimeF, vec3{ 0.f,1.f,0.f }); }
		SetUniform(basicShader, 1, mainCamera.transform.matrix);//Camera transform
		SetUniform(basicShader, 2, myGO.transform.matrix);//Cube transform
		SetUniform(basicShader, 7, vec3(mainCamera.transform.matrix[3]));//Cam position for specular. Mat4 to vec3 conversion
		DrawMesh(basicShader, myGO.mesh);
	}
	FreeMesh(myGO.mesh);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.terminate();
	return 0;
}