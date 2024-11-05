#pragma once
#include "glm/ext.hpp"//Vector math

struct AABB {
	/// <summary>
	/// The center position of this AABB
	/// </summary>
	glm::vec3 pos;
	/// <summary>
	/// The half extents of our object
	/// </summary>
	glm::vec3 size;
	AABB() {
		pos = glm::vec3(0, 0, 0);
		size = glm::vec3(1, 1, 1);
	}
	AABB(glm::vec3 pos, glm::vec3 size) {
		this->pos = pos;
		this->size = size;
	}
};