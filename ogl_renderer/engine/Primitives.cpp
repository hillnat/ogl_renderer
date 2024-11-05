#include "Primitives.h"
#include "../Renderer/Renderer.h"
#define PI 3.1415926535897932
namespace Primitives {
	Mesh Primitives::MakeSphere() {

		const float radius = 0.5f;
		const int longCount = 100;
		const int latCount = longCount * 0.5f;
		const int vertCount = (latCount + 1) + (longCount + 1);
		std::vector<vertex> verts;

		float x = 0, y = 0, z = 0, xy = 0, nx = 0, ny = 0, nz = 0, s = 0, t = 0, longAngle = 0, latAngle = 0;
		const float lengthInv = 1.0f / radius;
		const float longStep = 2.0f * PI / longCount;
		const float latStep = PI / latCount;

		for (int i = 0; i <= latCount; i++) {
			latAngle = (float)PI / 2.0f - (float)i * latStep;
			xy = radius * cosf(latAngle);
			z = radius * sin(latAngle);

			for (int j = 0; j <= longCount; j++) {
				//From 0 to 2pi
				longAngle = j * longStep;
				//Vert position
				x = xy * cosf(longAngle);
				y = xy * sinf(longAngle);

				vec4 pos = vec4(x, y, z, 1);
				//Normalize vector normal
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				vec3 normal = vec3(nx, ny, nz);

				s = (float)j / longCount;
				t = (float)i / latCount;

				vec2 uv = vec2(s, t);
				verts.push_back(vertex(pos, vec4(1, 1, 1, 1), normal, uv));
			}
		}
		std::vector<GLuint> indices;

		int k1, k2;
		for (int i = 0; i < latCount; ++i) {
			k1 = i * (longCount + 1);
			k2 = k1 + (longCount + 1);
			for (int j = 0; j < longCount; ++j, ++k1, ++k2) {
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (i != (latCount - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
		return MakeMesh(verts, indices);
	}
	//constexpr mesh MakeCylinder() {
	//	const int sides = 32;
	//	const float height = 1.0f;
	//	const float radius = .5f;
	//	const float sectorStep = 2 * PI / (float)sides;
	//
	//	vec3 unitCircleVertices[sides+1] = {};
	//	for (int i = 0; i <= sides; ++i) {
	//		float sectorAngle = i * sectorStep;
	//		unitCircleVertices[i]=(vec3(cos(sectorAngle), sin(sectorAngle), 0.0f));
	//	}
	//	const int vertCount = (2 * (sides + 1));
	//	vertex verts[vertCount] = {};
	//	
	//	for (int i = 0; i < 2; ++i) {
	//		float h = -height / 2.0f + (float)i * height;
	//		float t = 1.0f - (float)i;
	//		
	//		for (int j = 0, k = 0; j <= sides; ++j, k++) {
	//			float ux = unitCircleVertices[k].x;
	//			float uy = unitCircleVertices[k].y;
	//			float uz = unitCircleVertices[k].z;
	//			
	//			vec4 pos = vec4(ux * radius, uy * radius, h, 1);
	//			vec3 normal = vec3(ux, uy, uz);
	//			vec2 uv = vec2((float)j / sides, t);
	//			verts[i]=(vertex{ pos,vec4(1,1,1,1),normal,uv });
	//		}
	//	}
	//
	//	const int baseCenterIndex = vertCount;
	//	const int topCenterIndex = baseCenterIndex + sides + 1;
	//	//pikcup here
	//}
	Mesh Primitives::MakePlane() {
		vertex verts[4] = {
			vertex//top right
			(
				vec4(0.5f, 0.0f, 0.5f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(1.0f, 1.0f) //uv
			),
			vertex//top left
			(
				vec4(-0.5f, 0.0f, 0.5f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(0.0f, 1.0f) //uv
			),
			vertex//bottom right
			(
				vec4(0.5f, 0.0f, -0.5f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(1.0f, 0.0f) //uv
			),
			vertex//bottom left
			(
				vec4(-0.5f, 0.0f, -0.5f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(0.0f, 0.0f) //uv
			),
		};

		GLuint indices[] = {
			0,2,1, 3,1,2
		};

		return MakeMesh(verts, 4, indices, 6);
	}	
	//Mesh MakeCube() {
	//	vertex verts[24] = {
	//#pragma region Top Face
	//	vertex
	//	( //0 vertex Bottom Right corner
	//		vec4{1.f,  1.f,  1.f,  1}, //position
	//		CYAN, //vertex color
	//		vec3{0.f,1.f,0.f},  //vertex normal
	//		vec2{0.0f,1.0f}, //vertex uv
	//	),
	//	vertex
	//	{ //1 vertex Bottom Left corner
	//		{-1.f,  1.f,  1.f,  1}, //position
	//		CYAN, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{0.f,1.f,0.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{1.f,  1.f,  -1.f,  1}, //position
	//		CYAN, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{0.f,1.f,0.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{-1.f,  1.f,  -1.f,  1}, //position
	//		CYAN, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{0.f,1.f,0.f}  //vertex normal
	//	},
	//#pragma endregion
	//#pragma region Bottom Face
	//	{ //0 vertex Bottom Right corner
	//		{1.f,  -1.f,  1.f,  1}, //position
	//		BLUE, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{0.f,-1.f,0.f}  //vertex normal
	//	},
	//	{ //1 vertex Bottom Left corner
	//		{-1.f,  -1.f,  1.f,  1}, //position
	//		BLUE, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{0.f,-1.f,0.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{1.f,  -1.f,  -1.f,  1}, //position
	//		BLUE, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{0.f,-1.f,0.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{-1.f,  -1.f,  -1.f,  1}, //position
	//		BLUE, //vertex color
	//		{0.0f,1.0f}, //vertex uv
	//		{0.f,-1.f,0.f}  //vertex normal
	//	},
	//#pragma endregion
	//#pragma region Left Face
	//	{ //0 vertex Bottom Right corner
	//		{1.f,  1.f,  1.f,  1}, //position
	//		RED, //vertex color
	//		{0.0f,1.0f}, //vertex uv
	//		{1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //1 vertex Bottom Left corner
	//		{1.f,  -1.f,  1.f,  1}, //position
	//		RED, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{1.f,  1.f,  -1.f,  1}, //position
	//		RED, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{1.f,  -1.f,  -1.f,  1}, //position
	//		RED, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{1.f,0.f,0.f}  //vertex normal
	//	},
	//#pragma endregion
	//#pragma region Right Face
	//	{ //0 vertex Bottom Right corner
	//		{-1.f,  1.f,  1.f,  1}, //position
	//		YELLOW, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{-1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //1 vertex Bottom Left corner
	//		{-1.f,  -1.f,  1.f,  1}, //position
	//		YELLOW, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{-1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{-1.f,  1.f,  -1.f,  1}, //position
	//		YELLOW, //vertex color
	//		{0.0f,1.0f}, //vertex uv
	//		{-1.f,0.f,0.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{-1.f,  -1.f,  -1.f,  1}, //position
	//		YELLOW, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{-1.f,0.f,0.f}  //vertex normal
	//	},
	//#pragma endregion
	//#pragma region Rear Face
	//	{ //0 vertex Bottom Right corner
	//		{1.f,  1.f,  1.f,  1}, //position
	//		MAGENTA, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{0.f,0.f,1.f}  //vertex normal
	//	},
	//	{ //1 vertex Bottom Left corner
	//		{-1.f,  1.f,  1.f,  1}, //position
	//		MAGENTA, //vertex color
	//		{0.0f,1.0f}, //vertex uv
	//		{0.f,0.f,1.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{1.f,  -1.f,  1.f,  1}, //position
	//		MAGENTA, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{0.f,0.f,1.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{-1.f,  -1.f,  1.f,  1}, //position
	//		MAGENTA, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{0.f,0.f,1.f}  //vertex normal
	//	},
	//#pragma endregion
	//#pragma region Front Face
	//	{ //0 vertex Bottom Right corner
	//		{1.f,  1.f,  -1.f,  1}, //position
	//		GREEN, //vertex color
	//		{0.0f,1.0f}, //vertex uv
	//		{0.f,0.f,-1.f}  //vertex normal
	//	},
	//	{ //1 vertex Bottom Left corner
	//		{-1.f,  1.f,  -1.f,  1}, //position
	//		GREEN, //vertex color
	//		{1.0f,1.0f}, //vertex uv
	//		{0.f,0.f,-1.f}  //vertex normal
	//	},
	//	{ //2 vertex Top Right corner
	//		{1.f,  -1.f,  -1.f,  1}, //position
	//		GREEN, //vertex color
	//		{0.0f,0.0f}, //vertex uv
	//		{0.f,0.f,-1.f}  //vertex normal
	//	},
	//	{ //3 vertex Top Left corner
	//		{-1.f,  -1.f,  -1.f,  1}, //position
	//		GREEN, //vertex color
	//		{1.0f,0.0f}, //vertex uv
	//		{0.f,0.f,-1.f}  //vertex normal
	//	},
	//#pragma endregion
	//
	//	};
	//
	//	GLuint indices[] = {
	//		3,1,0,    0,2,3,
	//		7,4,5,    4,7,6,
	//
	//		11,8,9,   8,11,10,//Left
	//		15,13,12, 12,14,15,//Right
	//
	//		19,16,17, 16,19,18,//Rear
	//		23,21,20, 20,22,23//Front
	//		};
	//
	//	return MakeMesh(verts, 24, indices, 36);
	//}
	Mesh Primitives::MakeLine() {
		//Just a skinny double sided plane
		vertex verts[8] = {
			vertex//top right
			(
				vec4(0.05f, 0.0f, 1.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(1.0f, 1.0f) //uv
			),
			vertex//top left
			(
				vec4(-0.05f, 0.0f, 1.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(0.0f, 1.0f) //uv
			),
			vertex//bottom right
			(
				vec4(0.05f, 0.0f, 0.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(1.0f, 0.0f) //uv
			),
			vertex//bottom left
			(
				vec4(-0.05f, 0.0f, 0.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, 1.0f, 0.0f),  //normal
				vec2(0.0f, 0.0f) //uv
			),
			vertex//top right
			(
				vec4(0.05f, 0.0f, 1.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, -1.0f, 0.0f),  //normal
				vec2(1.0f, 1.0f) //uv
			),
			vertex//top left
			(
				vec4(-0.05f, 0.0f, 1.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, -1.0f, 0.0f),  //normal
				vec2(0.0f, 1.0f) //uv
			),
			vertex//bottom right
			(
				vec4(0.05f, 0.0f, 0.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, -1.0f, 0.0f),  //normal
				vec2(1.0f, 0.0f) //uv
			),
			vertex//bottom left
			(
				vec4(-0.05f, 0.0f, 0.0f, 1.0f), //position
				vec4(1.0f, 1.0f ,1.0f, 1.0f), //color
				vec3(0.0f, -1.0f, 0.0f),  //normal
				vec2(0.0f, 0.0f) //uv
			)
		};

		GLuint indices[] = {
			0,2,1, 3,1,2,
			4,5,6, 7,6,5
		};

		return MakeMesh(verts, 8, indices, 12);
	}
}