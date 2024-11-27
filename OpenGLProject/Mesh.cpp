#include "Mesh.h"

#include "glm/gtc/type_ptr.hpp"

void Mesh::CreateTriangle()
{
	 Vertices =
	{
		{0.f,0.f,0.f},{0,0,0},{1,1,1},
		{1,0,0},{1,1,1},{1,1,1},
		{0,0,1},{0,0,0},{1,1,1},
		{1,0,1},{1,1,1},{1,1,1},
		{0.5,1,0.5},{0,0,0},{1,1,1}

	};
	

	indicies =
	{
		{0,1,4},
		{0,2,4},
		{1,3,4},
		{2,3,4}
	};


	Binders.Init(Vertices);
	Binders.EBOInit(indicies);
	Binders.Bind();
	Binders.EBOBind();
	Vertex::BindAttributes();
	Binders.Unbind();
	Binders.EBOUnBind();
}

void Mesh::CreateCube()
{
	Vertices =
	{
	{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 1, 0)},
	// Vertex 2
	{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 1)},
	// Vertex 3
	{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.f), glm::vec3(1, 0, 0)},
	// Vertex 4
	{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 1, 0)},

	// Vertex 5
	{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 1, 0)},
	// Vertex 6
	{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 1)},
	// Vertex 7
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.f), glm::vec3(1, 0, 0)},
	// Vertex 8
	{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 1, 0)},


	};

	indicies =
	{
		{0,1,2},  // Front face
		{0,3,2},

		{7,4,5},  // Back face
		{7,5,6},

		{0,3,4},  // Top face
		{4,3,7},

		{1,2,5}, // Bottom face
		{6,2,5},

		{7,3,2}, // Left face
		{7,2,6},

		{0,4,5},  // Right face
		{0,5,1}
	};
	Binders.Init(Vertices);
	Binders.EBOInit(indicies);
	Binders.Bind();
	Binders.EBOBind();
	Vertex::BindAttributes();
	Binders.Unbind();
	Binders.EBOUnBind();
}

void Mesh::Createphere()
{


}

void Mesh::Draw(const char* uniform, Shader& shader)
{
	Binders.Bind();
	Binders.EBOBind();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
	//glDrawArrays(GL_POINTS, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, indicies.size()*3, GL_UNSIGNED_INT, nullptr);
}
