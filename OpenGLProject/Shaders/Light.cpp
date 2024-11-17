#include "Light.h"

#include "glm/gtc/type_ptr.inl"
#include "glm/gtx/transform.hpp"


Light::Light()
{
	lightColor = glm::vec3(1.0f, 1.f, 1.f);
	lightPos = glm::vec3(15.f, 15.f, 15.f);
	lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
}

void Light::DrawLight()
{

	
	
	

	
	

}
void Light::CreateLight( glm::vec3 scale, glm::vec3 color)
{

	GLfloat Matrice[] =
	{
		scale.x,scale.y,scale.z, 
		scale.x,-scale.y,scale.z, 
		scale.x,-scale.y,-scale.z, 
		scale.x,scale.y,-scale.z,

		-scale.x,scale.y,scale.z, 
		-scale.x,-scale.y,scale.z,
		-scale.x,-scale.y,-scale.z, 
		-scale.x,scale.y,-scale.z


	};

	unsigned int LightIndices[] =
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
	
	VAO LightVAO;
	LightVAO.Bind();

	VBO LightVBO(Matrice, sizeof(Matrice));
	LightVBO.Bind();

	EBO LightEBO(LightIndices, sizeof(LightIndices));
	LightEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LightVertex), (void*)0);
	glEnableVertexAttribArray(0);

	
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(lightModel));
	glDrawElements(GL_TRIANGLES, sizeof(LightIndices), GL_UNSIGNED_INT, nullptr);

	LightVAO.Unbind();
	LightVBO.Unbind();
	LightEBO.Unbind();

	LightVAO.Delete();
	LightVBO.Delete();
	LightEBO.Delete();

}
;