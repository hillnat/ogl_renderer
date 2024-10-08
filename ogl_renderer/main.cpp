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
void checkGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err << std::endl;
	}
}
int main()
{
	Context context;
	context.Initialize();
	Diagnostics::Environment();
//Scene Definitions
	Scene scene;
	//GameObject obj1("Player", "meshes/axisPointer.obj");
	GameObject obj1("Player", MakeSphere());
	obj1.transform.Translate(vec3(0, 0, -5));
	Camera mainCamera{};
	//mainCamera.transform.Rotate(vec3(1, 0, 0), 90.f);
	scene.AddToScene(&obj1);
	scene.AddToScene(&mainCamera);
	Light directionalLight1(vec3(1,1,1), vec3(0, -1, 0));
	scene.AddToScene(&directionalLight1);

	//Shaders
	//Shader basicShader = LoadShader("shaders/basic.vert", "shaders/basic.frag");
	//Shader lineShader = LoadShader("shaders/lines.vert", "shaders/lines.frag");
	Shader basicShader = LoadShader("shaders/basic.vert", "shaders/diffuse.frag");
	Texture basicTexture = LoadTexture("textures/wood.png");
	//cameraTransform = glm::lookAt(vec3{ cameraTransform[3] }, vec3{ myGeoTransform[3] }, { 0,1,0 });//From 0,0,0 look at 0,0,1 on axis 0,1,0
	const float MOVESPEED = 5.f;
	const float TURNSPEED = 80.f;
	const float LIGHTTURNSPEED = 80.f;
	double lastTime = 0;
	SetUniform(basicShader, 3, basicTexture, 0);
	//Test
	GameObject* myPlayer = &obj1;
	while (!context.ShouldClose()) {
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastTime;
		float deltaTimeF = (float)deltaTime;
		lastTime = nowTime;
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
		
		scene.DrawAll(&basicShader, deltaTimeF);
		checkGLError();
	}
	FreeMesh(obj1.mesh);
	FreeShader(basicShader);
	FreeTexture(basicTexture);
	context.Terminate();
	return 0;
}