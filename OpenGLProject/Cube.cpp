#include "Cube.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace glm;


Cube::Cube()
{

};

void Cube::DrawCube(glm::vec3 scale, glm::vec3 color, Shader& shader, const char* uniform)
{

	GLfloat Matrice[] =
	{
		1*scale.x,1*scale.y,1*scale.z, color.x,color.y,color.z, 0,1,0,
		1*scale.x,1*-scale.y,1*scale.z, color.x,color.y,color.z,0,0,1,
		1*scale.x,1*-scale.y,1*-scale.z, color.x,color.y,color.z,1,0,0,
		1*scale.x,1*scale.y,1*-scale.z, color.x,color.y,color.z,0,1,0,

		1*-scale.x,1*scale.y,1*scale.z, color.x,color.y,color.z,0,1,0,
		1*-scale.x,1*-scale.y,1*scale.z, color.x,color.y,color.z,0,0,1,
		1*-scale.x,1*-scale.y,1*-scale.z, color.x,color.y,color.z,1,0,0,
		1*-scale.x,1*scale.y,1*-scale.z, color.x,color.y,color.z,0,1,0

		

	};

	unsigned int CubeIndices[] =
	{
		0,1,2,  // Front face
		0,3,2,

		7,4,5,  // Back face
		7,5,6,

		0,3,4,  // Top face
		4,3,7,

		1,2,5, // Bottom face
		6,2,5,

		7,3,2, // Left face
		7,2,6,

		0,4,5,  // Right face
		0,5,1
	};



	//Draw Square
	VAO CubeVAO;
	CubeVAO.Bind();

	VBO CubeVBO(Matrice,sizeof(Matrice));
	CubeVBO.Bind();

	EBO CubeEBO(CubeIndices, sizeof(CubeIndices));
	CubeEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	CubeVAO.Unbind();
	CubeVBO.Unbind();
	CubeEBO.Unbind();

	CubeVAO.Bind();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CubeMatrix));
	glDrawElements(GL_TRIANGLES, sizeof(CubeIndices), GL_UNSIGNED_INT, nullptr);

	

	CubeVAO.Delete();
	CubeVBO.Delete();
	CubeEBO.Delete();
	AABB.Position = CubeMatrix[3];
	AABB.Extent = scale;
};

void Cube::CreateCube(glm::vec3 position, glm::vec3 scale, Shader& shader, const char* uniform)
{

	

}
glm::vec3 Cube::barycentricCoordinates( glm::vec3 p1,  glm::vec3 p2,  glm::vec3 p3,  glm::vec3 p4)
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
	vec3 nv = glm::cross(q, p);
	// double the area of p4pq
	baryc.y = nv.y / area_123;

	// w
	p = p1 - p4;
	q = p2 - p4;
	vec3 nw = (glm::cross(q, p));
	// double the area of p4pq
	baryc.z = nw.y / area_123;

	return baryc;
}
;


