#include "Primitives.h"
#include "../renderer/MeshImporter.h"
#define PI 3.1415926535897932

namespace Primitives {

	Mesh* MakeSphere() {

		const float radius = 0.5f;
		const int longCount = 15;
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
		return MeshImporter::MakeMesh(verts, indices);
	}

	Mesh* MakePlane() {
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

		return MeshImporter::MakeMesh(verts, 4, indices, 6);
	}

	GLuint CreateTextureArray(std::vector<Texture*> textures) {
		if (textures.empty()) return 0;

		GLuint textureArray;
		glGenTextures(1, &textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		// Get dimensions (assumes all textures are same size)
		int width = textures[0]->width;
		int height = textures[0]->height;
		int layers = textures.size();

		std::cout << "Creating texture array: " << width << "x" << height
			<< " with " << layers << " layers\n";

		// Allocate storage for all layers
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
			width, height, layers,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// Buffer to read pixel data from GPU
		std::vector<unsigned char> pixelBuffer(width * height * 4);

		// Upload each texture as a layer
		for (int i = 0; i < layers; i++) {
			// Bind the source texture to read from it
			glBindTexture(GL_TEXTURE_2D, textures[i]->handle);

			// Read pixels from the GPU texture
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer.data());

			// Bind back to texture array
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

			// Upload to array layer
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
				0, 0, i,  // offset and layer
				textures[i]->width, textures[i]->height, 1,
				GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer.data());

			std::cout << "  Uploaded layer " << i << "\n";
		}

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return textureArray;
	}

}
