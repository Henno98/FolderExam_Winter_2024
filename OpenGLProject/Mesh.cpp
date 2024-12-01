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

	CalculateNormals();

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
	CalculateNormals();
	Binders.Init(Vertices);
	Binders.EBOInit(indicies);
	Binders.Bind();
	Binders.EBOBind();
	Vertex::BindAttributes();
	Binders.Unbind();
	Binders.EBOUnBind();

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
	CalculateNormals();
	Binders.Init(Vertices);
	//Binders.EBOInit(indicies);
	Binders.Bind();
	//Binders.EBOBind();
	Vertex::BindAttributes();
	Binders.Unbind();
	//Binders.EBOUnBind();

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


}


void Mesh::CalculateNormals()
{
	float trianglenorm = 0;
	float verticenormal = 0;

	for (const auto& triangles : indicies)
	{
		const glm::vec3& v0 = Vertices[triangles.V0].position;
		const glm::vec3& v1 = Vertices[triangles.V1].position;
		const glm::vec3& v2 = Vertices[triangles.V2].position;

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 norm = glm::cross(edge1, edge2);

		glm::vec3 normals = glm::normalize(norm);

		Vertices[triangles.V0].normal += normals;
		Vertices[triangles.V1].normal += normals;
		Vertices[triangles.V2].normal += normals;
		trianglenorm++;

	}
	for (auto& vertice : Vertices)
	{
		vertice.normal = glm::normalize(vertice.normal);
		verticenormal++;
	}
	

}

void Mesh::Physics(Landscape& terrain, float deltatime)
{

	
	

	
	// Update sphere dynamics
	Acceleration = gravity;
	if (Velocity.y > -0.981f *Mass) {
		Velocity += (Acceleration);
	}
	Position += (Velocity );
	collider.Position = Position;
	Matrix = MatrixCalc();

}

void Mesh::Collision(Mesh& otheractor)
{

	glm::vec3 min = otheractor.collider.Position - otheractor.collider.Extent;
	glm::vec3 max = otheractor.collider.Position + otheractor.collider.Extent;
	glm::vec3 spheremin = Position;
	glm::vec3 spheremax = otheractor.Position;
	glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
	float diameter = glm::distance(spheremin, spheremax);
	glm::vec3 distance = spheremin - closestpoint;
	if (diameter <= Radius * 2 && diameter > 0) {

		
		//otheractor.SphereMatrix = glm::translate(otheractor.SphereMatrix, distance);
		glm::vec3 V1 = ((Mass - otheractor.Mass) / (Mass + otheractor.Mass) * Velocity)
			+ ((2 * otheractor.Mass) / (Mass + otheractor.Mass) * otheractor.Velocity);
		glm::vec3 V2 = ((Mass * 2) / (Mass + otheractor.Mass) * Velocity)
			+ ((otheractor.Mass - Mass) / (Mass + otheractor.Mass) * otheractor.Velocity);
		Velocity = V1;
		otheractor.Velocity = V2;
	}
}

void Mesh::Draw(const char* uniform, Shader& shader)
{
	
	Binders.Bind();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
	switch (mesh) {
	case Pyramid:
		Binders.EBOBind();
		glDrawElements(GL_TRIANGLES, indicies.size() * 3, GL_UNSIGNED_INT, nullptr);
		break;
	case Cube:
		Binders.EBOBind();
		glDrawElements(GL_TRIANGLES, indicies.size() * 3, GL_UNSIGNED_INT, nullptr);
		break;
	case Sphere:
		glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
		break;
	default:
		std::cout << "Mesh type does not exist" << std::endl;
	}
	
	
}
