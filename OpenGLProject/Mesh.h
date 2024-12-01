#pragma once
#include <iostream>
#include <vector>

#include "Landscape.h"
#include "Vertex.h"
#include "Dependencies/includes/glm/glm.hpp"
#include "../Shaders/ObjectBinders.h"
#include "Models/BoundingBox.h"
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
	BoundingBox collider;
	glm::mat4 Matrix = glm::mat4(1.f);
	MeshType mesh;
	float Mass{4.f};
	glm::vec3 Position = glm::vec3(0.f);
	glm::vec3 Rotation = glm::vec3(0.f);
	glm::vec3 Transform = glm::vec3(0.f);
	glm::vec3 Acceleration = glm::vec3(-1.f);
	glm::vec3 Velocity = glm::vec3(0.f);
	glm::vec3 gravity = glm::vec3(0, -0.0981f, 0);
	float Friction{0.998f};
	float Radius{1.f};

	Mesh(MeshType meshtype)
	{
		mesh = meshtype;
		switch (mesh) {
		case Pyramid:
			CreateTriangle();
			break;
		case Cube:
			CreateCube();
			break;
		case Sphere:
			CreateSphere(3,1.f,glm::vec3(1.f));
			break;
		default:
			std::cout << "Mesh type does not exist" << std::endl;
		}
	};

	void CreateTriangle();
	void CreateCube();
	void CreateSphere(int subdivisions, float scale,glm::vec3 speed);
	void Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n);
	void CreateTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
	void Diamond();
	void CalculateNormals();
	void Physics(Landscape& terrain, float deltatime);
	void Collision(Mesh& otheractor);
	void Draw(const char* uniform, Shader& shader);
	glm::mat4 MatrixCalc()
	{
		glm::mat4 newMatrix = glm::mat4(1.f);
		newMatrix = glm::translate(glm::mat4(1.f), Position);
		//std::cout << "Calculated new matrix pos" << Position.x <<" " <<  Position.y << " " << Position.z << endl;
		return newMatrix;
	}
};

