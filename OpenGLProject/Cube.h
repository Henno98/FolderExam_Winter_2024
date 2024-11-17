#pragma once
#include "vector"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"

#ifndef CUBE_H
#define CUBE_H

struct CubeVertex
{
	float x, y, z;
	float r, g, b;
	float n1, n2, n3;
};


class Cube
{
public:
	glm::mat4 CubeMatrix = glm::mat4(1.0f);
	BoundingBox AABB;
	int collectedtrophies;
	Cube();
	void DrawCube(glm::vec3 scale, glm::vec3 color, Shader& shader, const char* uniform);
	void CreateCube(glm::vec3 position, glm::vec3 scale, Shader& shader, const char* uniform);

	glm::vec3 barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);

	
};


#endif // CUBE_H