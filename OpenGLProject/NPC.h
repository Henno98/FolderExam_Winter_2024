#pragma once
#include <vector>
#include <Eigen/Dense>
#include "BoundingBox.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "Shaders/ShaderClass.h"

struct NPCVertex
{
	GLfloat x, y, z, r, g, b,n1,n2,n3;
};
class NPC
{

public:

	glm::mat4 NPCMatrix = glm::mat4(1.0f);
	std::vector<NPCVertex> Movementpoints;
	BoundingBox NPCBox;
	glm::vec3 Color;
	glm::vec3 currentposition;
	glm::vec3 Targetposition;
	glm::vec3 Oldposition;
	glm::vec3 Newposition;
	NPC();
	BoundingBox AABB;
	Eigen::MatrixXd AMatrix=Eigen::MatrixXd(4, 4);
	Eigen::MatrixXd YMatrix=Eigen::MatrixXd(4, 1);
	bool changefunction = false;
	bool turn = false;
	float x=0.f;
	glm::vec3 pos1;
	glm::vec3 pos2;
	glm::vec3 pos3;
	glm::vec3 pos4;
	void Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, int range);
	glm::vec3 barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	void DrawNPC( Shader& shader, const char* uniform);
	std::vector<NPCVertex> CalculateMovement(Eigen::MatrixXd solution, int range);
	void UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation);
};

