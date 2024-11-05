#pragma once
#include "renderer.h"

class Material
{
public:
	Material(Shader* shader) {
		this->shader = shader;
	}
	Shader* shader;
	/// <summary>
	/// Sets the texture of this material / shader
	/// </summary>
	/// <param name="tex">Texture to use</param>
	void SetTexture(Texture* tex);
};