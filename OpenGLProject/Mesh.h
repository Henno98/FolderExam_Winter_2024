#pragma once
#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Dependencies/includes/glm/glm.hpp"
#include "../Shaders/ObjectBinders.h"
#include "Shaders/ShaderClass.h"
enum MeshType
{

	Pyramid,
	Cube,
	Sphere,
	unknown

};
class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<Indices> indicies;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	
	Mesh(MeshType meshtype)
	{
		
		switch (meshtype) {
		case Pyramid:
			CreateTriangle();
			break;
		case Cube:
			CreateCube();
			break;
		case Sphere:
			Createphere();
			break;
		default:
			std::cout << "Mesh type does not exist" << std::endl;
		}
	};

	void CreateTriangle();
	void CreateCube();
	void Createphere();
	void Diamond();
	void Draw(const char* uniform, Shader& shader);
};

