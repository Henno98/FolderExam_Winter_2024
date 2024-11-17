#include "ObjectBinders.h"



ObjectBinders::ObjectBinders()
{
}

void ObjectBinders::Init(GLfloat* vertices, GLsizeiptr size)
{
	glGenVertexArrays(1, &ID);
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void ObjectBinders::EBOInit(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void ObjectBinders::Bind()
{
	glBindVertexArray(ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void ObjectBinders::EBOBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ObjectBinders::EBOUnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ObjectBinders::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ObjectBinders::Delete()
{
	glDeleteVertexArrays(1, &ID);
	glDeleteBuffers(1, &ID);
}

