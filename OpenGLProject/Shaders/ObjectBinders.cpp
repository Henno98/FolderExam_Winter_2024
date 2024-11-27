#include "ObjectBinders.h"



ObjectBinders::ObjectBinders()
{
}

void ObjectBinders::Init(std::vector<Vertex>& Vertices)
{
	glGenVertexArrays(1, &VAOID);
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);
}

void ObjectBinders::EBOInit(std::vector<Indices>& indices)
{
	glGenBuffers(1, &EBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBOID );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(Indices), indices.data(), GL_STATIC_DRAW);
}

void ObjectBinders::Bind()
{
	glBindVertexArray(VAOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
}

void ObjectBinders::EBOBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
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
	glDeleteVertexArrays(1, &VAOID);
	glDeleteBuffers(1, &VBOID);
}

