#include "Material.h"
#include "Renderer.h"

void Material::SetTexture(Texture* tex) {
	SetUniform(shader, (GLuint)3, tex, 0);//Attach texture to shader
}
