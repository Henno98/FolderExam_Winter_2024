#include "NPC.h"

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "../Shaders/EBO.h"
#include "../Shaders/VAO.h"
using namespace Eigen;
//GLuint NPCIndices[]
//{
//	0,1,2,
//	0,2,3,
//	3,4,2,
//	4,2,1
//};



NPC::NPC()
{
	Color = glm::vec3(1, 0, 1);
	
}



void NPC::DrawNPC(Shader& shader, const char* uniform)
{
	GLfloat Indices[] =
	{
		0.f,0.f,0.f,0,0,0,1,1,1,
		1,0,0,1,1,1,1,1,1,
		0,0,1,0,0,0,1,1,1,
		1,0,1,1,1,1,1,1,1,
		0.5,1,0.5,0,0,0,1,1,1

	};

	GLuint NPCIndices[]
	{
		0,1,4,
		0,2,4,
		1,3,4,
		2,3,4
	};

	
	//Draw Square
	VAO NPCVAO;
	NPCVAO.Bind();

	VBO NPCVBO(Indices, sizeof(Indices));
	NPCVBO.Bind();

	EBO NPCEBO(NPCIndices, sizeof(NPCIndices));
	NPCEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int modelloc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(NPCMatrix));

	glDrawElements(GL_TRIANGLES, sizeof(NPCIndices), GL_UNSIGNED_INT, nullptr);

	NPCVAO.Unbind();
	NPCVBO.Unbind();
	NPCEBO.Unbind();

	NPCVAO.Delete();
	NPCVBO.Delete();
	NPCEBO.Delete();

	AABB.Position = NPCMatrix[3];
	AABB.Extent = glm::vec3(0.5, 0.5, 0.5);
}


void NPC::Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, int range)
{
	

	//double x0 = pos1.x;
	//double x1 = pow(pos1.x, 3);
	//double x2 = pow(pos1.x, 2);

	//double x02 = pos2.x;
	//double x12 = pow(pos2.x, 3);
	//double x22 = pow(pos2.x, 2);

	//double x03 = pos3.x;
	//double x13 = pow(pos3.x, 3);
	//double x23 = pow(pos3.x, 2);

	//double x04 = pos4.x;
	//double x14 = pow(pos4.x, 3);
	//double x24 = pow(pos4.x, 2);

	//AMatrix <<
	//	x1, x2, x0, 1.0,
	//	x12, x22, x02, 1.0,
	//	x13, x23, x03, 1.0,
	//	x14, x24, x04, 1.0;

	//YMatrix <<
	//	pos1.z,
	//	pos2.z,
	//	pos3.z,
	//	pos4.z;

	//MatrixXd AInverse = AMatrix.inverse();
	//MatrixXd SolutionMatrix = AInverse * YMatrix;


	//float z = (SolutionMatrix(0, 0) * pow(x, 3)) + (SolutionMatrix(1, 0) * pow(x, 2)) + (SolutionMatrix(2, 0) * SolutionMatrix(3, 0));
	////std::cout << "for X value: " << x << " Z value is: " << z << std::endl;

	/*if (!turn == true) {
		NPCMatrix = glm::translate(NPCMatrix, glm::vec3(x / 1000, 0, z / 1000));
		x += 0.1f;
		if (x >= 10)
		{
			turn = true;
		}

	}
	if (!turn == false)
	{
		NPCMatrix = glm::translate(NPCMatrix, glm::vec3(-x / 1000, 0, -z / 1000));
		x -= 0.1f;
		if (x <= 0)
		{
			turn = false;
		}
	}
	NPCMatrix = glm::translate(NPCMatrix, glm::vec3(x - NPCMatrix[3].x, 0, z - NPCMatrix[3].z));*/




}

glm::vec3 NPC::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{

	glm::vec3 p12 = p2 - p1;
	glm::vec3 p13 = p3 - p1;
	glm::vec3 cross = glm::cross(p13, p12);
	float area_123 = cross.y; // double the area
	glm::vec3 baryc; // for return

	// u
	glm::vec3 p = p2 - p4;
	glm::vec3 q = p3 - p4;
	glm::vec3 nu = glm::cross(q, p);
	// double the area of p4pq
	baryc.x = nu.y / area_123;

	// v
	p = p3 - p4;
	q = p1 - p4;
	glm::vec3 nv = glm::cross(q, p);
	// double the area of p4pq
	baryc.y = nv.y / area_123;

	// w
	p = p1 - p4;
	q = p2 - p4;
	glm::vec3 nw = (glm::cross(q, p));
	// double the area of p4pq
	baryc.z = nw.y / area_123;

	return baryc;
}
;

