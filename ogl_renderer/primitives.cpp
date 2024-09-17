#include "primitives.h"
/*
#include "renderer.h"

Mesh MakeCube() {
	Vertex verts[] = {
	{ //0 vertex Bottom Left Back
		{-1.f,-1.f,1.f,1}, //position
		{1,0,0,1}, //vertex color
		{1.0f,0.0f}, //vertex uv
		{-.7,-.7,.7}  //vertex normal
	},
	{ //1 vertex Bottom Right Back
		{1.f,-1.f,1.f,1}, //position
		{1,1.f,0,1}, //vertex color
		{0.0f,0.0f}, //vertex uv
		{.7,-.7,.7}  //vertex normal
	},
	{ //2 vertex Top Left Back
		{-1.f,1.f,1.f,1}, //position
		{1,1,1,1}, //vertex color
		{1.0f,1.0f}, //vertex uv
		{-0.7,0.7,0.7}  //vertex normal
	},
	{ //3 vertex Top Right Back
		{1.f,1.f,1.f,1}, //position
		{1,1,1,1}, //vertex color
		{0.0f,1.0f}, //vertex uv
		{0.7,0.7,0.7}  //vertex normal
	},
	{ //4 vertex Bottom Right Front
		{1.f,-1.f,-1.f,1}, //position
		{1,1,0,1}, //vertex color
		{1.0f,0.0f}, //vertex uv
		{0.7,-.7,-.7}  //vertex normal
	},
	{ //5 vertex Top Right Front
		{1.f,1.f,-1.f,1}, //position
		{1,1,1,1}, //vertex color
		{1.0f,1.0f}, //vertex uv
		{.7,.7,-.7}  //vertex normal
	},
	{ //6 vertex Bottom Left Front
		{-1.f,-1.f,-1.f,1}, //position
		{0,1,0,1}, //vertex color
		{0.0f,0.0f}, //vertex uv
		{-.7,-.7,-.7}  //vertex normal
	},
	{ //7 vertex Top Left Front
		{-1.f,1.f,-1.f,1}, //position
		{0,1,1,1}, //vertex color
		{0.0f,1.0f}, //vertex uv
		{-.7,.7,-.7}  //vertex normal
	}

	};

	GLuint indices[] = {
		0,1,2, 2,1,3,
		1,4,3, 3,4,5,
		4,6,5, 5,6,7,
		6,0,7, 7,0,2,
		2,3,7, 7,3,5,
		6,4,0, 0,4,1 };

	return MakeMesh(verts, 8, indices, 24);
}
Mesh MakeCube2() {
	Vertex verts[] = {
#pragma region Top Face
	{ //0 vertex Bottom Right corner
		{1.f,  1.f,  1.f,  1}, //position
		CYAN, //vertex color
		{0.0f,1.0f}, //vertex uv
		{0.f,1.f,0.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{-1.f,  1.f,  1.f,  1}, //position
		CYAN, //vertex color
		{1.0f,1.0f}, //vertex uv
		{0.f,1.f,0.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{1.f,  1.f,  -1.f,  1}, //position
		CYAN, //vertex color
		{0.0f,0.0f}, //vertex uv
		{0.f,1.f,0.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{-1.f,  1.f,  -1.f,  1}, //position
		CYAN, //vertex color
		{1.0f,0.0f}, //vertex uv
		{0.f,1.f,0.f}  //vertex normal
	},
#pragma endregion
#pragma region Bottom Face
	{ //0 vertex Bottom Right corner
		{1.f,  -1.f,  1.f,  1}, //position
		BLUE, //vertex color
		{1.0f,0.0f}, //vertex uv
		{0.f,-1.f,0.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{-1.f,  -1.f,  1.f,  1}, //position
		BLUE, //vertex color
		{0.0f,0.0f}, //vertex uv
		{0.f,-1.f,0.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{1.f,  -1.f,  -1.f,  1}, //position
		BLUE, //vertex color
		{1.0f,1.0f}, //vertex uv
		{0.f,-1.f,0.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{-1.f,  -1.f,  -1.f,  1}, //position
		BLUE, //vertex color
		{0.0f,1.0f}, //vertex uv
		{0.f,-1.f,0.f}  //vertex normal
	},
#pragma endregion
#pragma region Left Face
	{ //0 vertex Bottom Right corner
		{1.f,  1.f,  1.f,  1}, //position
		RED, //vertex color
		{0.0f,1.0f}, //vertex uv
		{1.f,0.f,0.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{1.f,  -1.f,  1.f,  1}, //position
		RED, //vertex color
		{0.0f,0.0f}, //vertex uv
		{1.f,0.f,0.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{1.f,  1.f,  -1.f,  1}, //position
		RED, //vertex color
		{1.0f,1.0f}, //vertex uv
		{1.f,0.f,0.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{1.f,  -1.f,  -1.f,  1}, //position
		RED, //vertex color
		{1.0f,0.0f}, //vertex uv
		{1.f,0.f,0.f}  //vertex normal
	},
#pragma endregion
#pragma region Right Face
	{ //0 vertex Bottom Right corner
		{-1.f,  1.f,  1.f,  1}, //position
		YELLOW, //vertex color
		{1.0f,1.0f}, //vertex uv
		{-1.f,0.f,0.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{-1.f,  -1.f,  1.f,  1}, //position
		YELLOW, //vertex color
		{1.0f,0.0f}, //vertex uv
		{-1.f,0.f,0.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{-1.f,  1.f,  -1.f,  1}, //position
		YELLOW, //vertex color
		{0.0f,1.0f}, //vertex uv
		{-1.f,0.f,0.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{-1.f,  -1.f,  -1.f,  1}, //position
		YELLOW, //vertex color
		{0.0f,0.0f}, //vertex uv
		{-1.f,0.f,0.f}  //vertex normal
	},
#pragma endregion
#pragma region Rear Face
	{ //0 vertex Bottom Right corner
		{1.f,  1.f,  1.f,  1}, //position
		MAGENTA, //vertex color
		{1.0f,1.0f}, //vertex uv
		{0.f,0.f,1.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{-1.f,  1.f,  1.f,  1}, //position
		MAGENTA, //vertex color
		{0.0f,1.0f}, //vertex uv
		{0.f,0.f,1.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{1.f,  -1.f,  1.f,  1}, //position
		MAGENTA, //vertex color
		{1.0f,0.0f}, //vertex uv
		{0.f,0.f,1.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{-1.f,  -1.f,  1.f,  1}, //position
		MAGENTA, //vertex color
		{0.0f,0.0f}, //vertex uv
		{0.f,0.f,1.f}  //vertex normal
	},
#pragma endregion
#pragma region Front Face
	{ //0 vertex Bottom Right corner
		{1.f,  1.f,  -1.f,  1}, //position
		GREEN, //vertex color
		{0.0f,1.0f}, //vertex uv
		{0.f,0.f,-1.f}  //vertex normal
	},
	{ //1 vertex Bottom Left corner
		{-1.f,  1.f,  -1.f,  1}, //position
		GREEN, //vertex color
		{1.0f,1.0f}, //vertex uv
		{0.f,0.f,-1.f}  //vertex normal
	},
	{ //2 vertex Top Right corner
		{1.f,  -1.f,  -1.f,  1}, //position
		GREEN, //vertex color
		{0.0f,0.0f}, //vertex uv
		{0.f,0.f,-1.f}  //vertex normal
	},
	{ //3 vertex Top Left corner
		{-1.f,  -1.f,  -1.f,  1}, //position
		GREEN, //vertex color
		{1.0f,0.0f}, //vertex uv
		{0.f,0.f,-1.f}  //vertex normal
	},
#pragma endregion

	};

	GLuint indices[] = {
		3,1,0,    0,2,3,
		7,4,5,    4,7,6,

		11,8,9,   8,11,10,//Left
		15,13,12, 12,14,15,//Right

		19,16,17, 16,19,18,//Rear
		23,21,20, 20,22,23//Front
		};

	return MakeMesh(verts, 24, indices, 36);
}*/