#include "Light.h"

#include "glm/gtc/type_ptr.inl"
#include "glm/gtx/transform.hpp"


Light::Light()
{
	lightColor = glm::vec3(1.0f, 1.f, 1.f);
	lightPos = glm::vec3(40.f, 15.f, 40.f);
	lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
}

void Light::DrawLight()
{

	
	
	

	
	

}
void Light::CreateLight( glm::vec3 scale, glm::vec3 color)
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
;