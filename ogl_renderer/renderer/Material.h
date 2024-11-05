#pragma once
#include "renderer.h"

class Material
{
public:
	Material(Shader* shader) {
		this->shader = shader;
	}
	Shader* shader;
	void SetTexture(Texture* tex);
};