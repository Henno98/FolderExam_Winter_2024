#pragma once
#include "../Dependencies/includes/glad/glad.h"
#include "../Vertex.h"
#include "vector"
class ObjectBinders
{
public:
	ObjectBinders();
	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	void Init(std::vector<Vertex>& Vertices);

	void EBOInit(std::vector<Indices>& indices);

	void Bind();

	void EBOBind();

	void ReBind(std::vector<Vertex>& Vertices);

	void EBOReBind(std::vector<Indices>& indices);

	void EBOUnBind();

	void Unbind();

	void Delete();


};

