#include "Trophy.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"

Trophy::Trophy()
{
	Color = glm::vec3(0.f, 1.f, 1.f);
	trophycollected = false;
}
void Trophy::PlaceTrophy(glm::vec3 position, glm::vec3 size)
{


	if (!placed == false)
		return;

	TrophyMatrix = glm::translate(TrophyMatrix, position);
	placed = true;
	
	

}

void Trophy::DrawTrophy( glm::vec3 size, Shader& shader, const char* uniform)
{
	if (!trophycollected == false)
		return;


	GLfloat Vertices[] = 
	{
		0,-size.y,0.f,Color.x,Color.y,Color.z, 1,0,0,
		size.x,0.f,size.z,0,1,0,0,1,0,
		-size.x,0,size.z,1,0,1,0,0,1,
		size.x,0,-size.z,0,1,0,1,0,0,
		-size.x,0,-size.z,1,0,1,0,1,0,
		0,size.y,0.f,Color.x,Color.y,Color.z,0,0,1



	};

	GLuint trophyIndices[]
	{

		0,1,2,
		0,2,4,
		0,4,3,
		0,3,1,
		5,1,2,
		5,2,4,
		5,4,3,
		5,3,1,

	};

	
	

	//Draw Square
	VAO trophyVAO;
	trophyVAO.Bind();

	VBO trophyVBO(Vertices,sizeof(Vertices));
	trophyVBO.Bind();

	EBO trophyEBO(trophyIndices, sizeof(trophyIndices));
	trophyEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int modelloc = glGetUniformLocation(shader.ID, uniform);
	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(TrophyMatrix));

	
	AABB.Position = TrophyMatrix[3];
	AABB.Extent = size;

	glDrawElements(GL_TRIANGLES, sizeof(trophyIndices), GL_UNSIGNED_INT, nullptr);

	trophyVAO.Unbind();
	trophyVBO.Unbind();
	trophyEBO.Unbind();

	trophyVAO.Delete();
	trophyVBO.Delete();
	trophyEBO.Delete();
	AABB.Position = TrophyMatrix[3];
	AABB.Extent = size;
}

void Trophy::DestroyTrophy()
{
	trophycollected = true;
}

glm::vec3 Trophy::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
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