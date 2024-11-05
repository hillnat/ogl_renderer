#pragma once
#include "glm/ext.hpp"
#include "../physics/AABB.h"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::quat;
using glm::identity;
#include <iostream>
class Transform
{
public:
#pragma region Constructors  
	Transform() {
		this->matrix = identity<mat4>();
		this->parent = nullptr;
	}
#pragma endregion
	/// <summary>
	/// This transforms parent transform
	/// </summary>
	Transform* parent = nullptr;
	/// <summary>
	/// This transforms matrix
	/// </summary>
	mat4 matrix = identity<mat4>();
	/// <returns> Our local right vector </returns>
	vec3 Right() { return vec3(matrix[0]); }
	/// <returns> Our local up vector </returns>
	vec3 Up() { return vec3(matrix[1]); }
	/// <returns> Our local forward vector </returns>
	vec3 Forward() { return vec3(matrix[2]); }
	/// <summary> Translates our matrix, factoring in our rotation </summary>
	/// <param name="translation">The vector which our matrix will be translated by</param>
	void TranslateLocal(vec3);
	/// <summary> Translates our matrix, disregarding rotation </summary>
	/// <param name="translation"> The vector which our matrix will be translated by</param>
	void TranslateGlobal(vec3 translation);
	/// <summary>Rotates our matrix</summary>
	/// <param name="axis">The axis to rotate about</param>
	/// <param name="degrees">Amount to rotate in degrees</param>
	void Rotate(vec3 axis,float degrees);
	/// <summary> Set the rotation of our matrix </summary>
	/// <param name="axis">The axis to rotate about</param>
	/// <param name="degrees">Amount to rotate in degrees</param>
	void SetRotation(vec3 axis,float degrees);
	/// <summary>
	/// Gets the position of this transform
	/// </summary>
	/// <returns>Position of matrix</returns>
	vec3 GetPosition();
	/// <summary> Applis a scalar to our matrix </summary>
	void ChangeScale(vec3 scalar);
	/// <summary> Points this transform at a given point </summary>
	void LookAt(vec3 pos);
	/// <summary>
	/// Output the values of matrix
	/// </summary>
	void LogTransform();
	///<summary> Recursivley grabs the position of all parents and returns the sum </summary>
	/// <returns> The combined positions of all our parents </returns>
	vec3 GetAccumParentPosition(Transform* transform);
};
