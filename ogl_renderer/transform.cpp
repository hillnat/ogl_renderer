#include "transform.h"
void Transform::Translate(vec3 vec) {
	matrix = glm::translate(matrix, vec);
}
void Transform::TranslateL(vec3 vec) {
	matrix = glm::translate(matrix, vec3{ matrix[0][0],matrix[1][1],matrix[2][2], } *vec);
}
void Transform::Rotate(float angle, vec3 axis) {
	matrix = glm::rotate(matrix, angle, axis);
}
void Transform::LookAt(vec3 lookAt, vec3 axis) {
	matrix = glm::lookAt(vec3{ matrix[3]}, lookAt, axis);
}