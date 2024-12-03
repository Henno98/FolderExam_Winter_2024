#pragma once
#include "glm/fwd.hpp"
#include "vector"
#include "ObjectBinders.h"
#include "ShaderClass.h"
#include <glm/glm.hpp>


struct LightVertex
{
	float x, y, z;
	
};


class Light
{
public:
	ObjectBinders Binders;
	std::vector<Vertex> Vertices;
	std::vector<Indices> indicies;
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	glm::mat4 lightModel = glm::mat4(1.0f);
	
	Light();
	void DrawLight();
	void CreateLight(glm::vec3 scale, glm::vec3 color);
};





