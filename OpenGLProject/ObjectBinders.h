#pragma once
#include "../Dependencies/includes/glad/glad.h"
class ObjectBinders
{
public:
	ObjectBinders();
	GLuint VAOID;
	GLuint VBOID;
	GLuint EBOID;
	void Init(GLfloat* vertices, GLsizeiptr size);

	void EBOInit(GLuint* indices, GLsizeiptr size);

	void Bind();

	void EBOBind();

	void EBOUnBind();

	void Unbind();

	void Delete();


};

