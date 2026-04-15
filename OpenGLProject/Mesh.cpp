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

	CalculateNormals(indicies, Vertices);

	Binders.Init(Vertices, indicies);
	Binders.Bind();
	Binders.Unbind();
	
}

void Mesh::CreateCube()
{
	Vertices =
	{
	{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 2
	{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 3
	{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 4
	{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},

	// Vertex 5
	{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 6
	{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 7
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},
	// Vertex 8
	{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.f), glm::vec3(0, 0, 0)},

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
	CalculateNormals(indicies, Vertices);
	Binders.Init(Vertices,indicies);
	Binders.Bind();
	Binders.Unbind();

	collider.Extent = glm::vec3(1.f);
}

void Mesh::CreateSphere(int subdivisions, float scale, glm::vec3 speed)
{
	glm::vec3 v0(0, 0, 1);
	glm::vec3 v1(1, 0, 0);
	glm::vec3 v2(0, 1, 0);
	glm::vec3 v3(-1, 0, 0);
	glm::vec3 v4(0, -1, 0);
	glm::vec3 v5(0, 0, -1);

	Radius = scale;
	
	Subdivide(v0, v1, v2, subdivisions);
	Subdivide(v0, v2, v3, subdivisions);
	Subdivide(v0, v3, v4, subdivisions);
	Subdivide(v0, v4, v1, subdivisions);

	Subdivide(v5, v2, v1, subdivisions);
	Subdivide(v5, v3, v2, subdivisions);
	Subdivide(v5, v4, v3, subdivisions);
	Subdivide(v5, v1, v4, subdivisions);
	for (auto& Vertex : Vertices)
	{
		Vertex.position *= scale;
	}
	CalculateNormals(indicies,Vertices);
	Binders.Init(Vertices, indicies);
	Binders.Bind();
	Binders.Unbind();
	collider.Extent = glm::vec3(scale);

}
void Mesh::Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n)
{
	if (n > 0)
	{
		glm::vec3 v1 = a + b;
		v1 = glm::normalize(v1);
		glm::vec3 v2 = a + c;
		v2 = glm::normalize(v2);
		glm::vec3 v3 = c + b;
		v3 = glm::normalize(v3);
		Subdivide(a, v1, v2, n - 1);
		Subdivide(c, v2, v3, n - 1);
		Subdivide(b, v3, v1, n - 1);
		Subdivide(v3, v2, v1, n - 1);
	}
	else
	{
		CreateTriangle(a, b, c);

	}
}

void Mesh::CreateTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	Vertex V{ glm::vec3(v1.x,v1.y,v1.z),glm::vec3(1.f),glm::vec3(1.f)};
	Vertices.emplace_back(V);
	V = Vertex{ glm::vec3(v2.x,v2.y,v2.z),glm::vec3(1.f),glm::vec3(1.f) };
	Vertices.emplace_back(V);
	V = Vertex{ glm::vec3(v3.x,v3.y,v3.z),glm::vec3(1.f),glm::vec3(1.f) };
	Vertices.emplace_back(V);

	collider.Extent = glm::vec3(1.f);
}


void Mesh::CustomCreateTriangle(std::vector<Vertex>& vertices, std::vector<Indices>& indices)
{
	Vertices = vertices;
	indicies = indices;
	CalculateNormals(indices,vertices);
	Binders.Init(Vertices, indicies);
	Binders.Bind();
	Binders.Unbind();
	

}

void Mesh::CustomCreateSpline(std::vector<Vertex>& vertices)
{
	Vertices = vertices;
	CalculateNormals(indicies, Vertices);
	Binders.Init(Vertices, indicies);
	Binders.Bind();
	Binders.Unbind();
}

void Mesh::ReBind(std::vector<Vertex>& vertices)
{
	Binders.ReBind(vertices);
}

void Mesh::CalculateNormals(std::vector<Indices>& indices, std::vector<Vertex>& vertices)
{
	float trianglenorm = 0;
	float verticenormal = 0;

	/*for (auto& vertice : vertices)
	{
		vertice.normal = glm::vec3(0.f);

	}*/

	for (const auto& triangles : indices)
	{
		const glm::vec3& v0 = vertices[triangles.V0].position;
		const glm::vec3& v1 = vertices[triangles.V1].position;
		const glm::vec3& v2 = vertices[triangles.V2].position;

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 norm = glm::cross(edge1, edge2);

		glm::vec3 normals = glm::normalize(norm);


		vertices[triangles.V0].normal += normals;
		vertices[triangles.V1].normal += normals;
		vertices[triangles.V2].normal += normals;
		trianglenorm++;
		/*vertices[triangles.V0].Color += normals;
		vertices[triangles.V1].Color += normals;
		vertices[triangles.V2].Color += normals;*/

	}
	for (auto& vertice : vertices)
	{
		vertice.normal = glm::normalize(vertice.normal);
		verticenormal++;
	}
	
}


void Mesh::Draw(const char* uniform, Shader& shader)
{
	
	Binders.Bind();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
	switch (mesh) {
	case Pyramid:
		
		glDrawElements(GL_TRIANGLES, indicies.size() * 3, GL_UNSIGNED_INT, nullptr);
		break;
	case Cube:
		
		glDrawElements(GL_TRIANGLES, indicies.size() * 3, GL_UNSIGNED_INT, nullptr);
		break;
	case Sphere:
		glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
		break;
	case Custom:
		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, nullptr);
		break;
	case Line:
		glLineWidth(10.f);
		glDrawArrays(GL_LINE_STRIP, 0, Vertices.size());
		break;
	default:
		std::cout << "Mesh type does not exist" << std::endl;
	}
	
	
}
