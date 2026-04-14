#include "ObjectBinders.h"



ObjectBinders::ObjectBinders()
{
}
void ObjectBinders::Init(std::vector<Vertex>& Vertices)
{
	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);
	// VBO
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER,
		Vertices.size() * sizeof(Vertex),
		Vertices.data(),
		GL_STATIC_DRAW);
	Vertex::BindAttributes();
	// unbind VAO LAST (EBO stays stored in VAO state!)
	glBindVertexArray(0);
}

void ObjectBinders::Init(std::vector<Vertex>& Vertices,
    std::vector<Indices>& indices)
{
	
    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);

    // VBO
    glGenBuffers(1, &VBOID);
    glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    glBufferData(GL_ARRAY_BUFFER,
        Vertices.size() * sizeof(Vertex),
        Vertices.data(),
        GL_STATIC_DRAW);

    
    glGenBuffers(1, &EBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(Indices),
        indices.data(),
        GL_STATIC_DRAW);

    
    Vertex::BindAttributes();

    // unbind VAO LAST (EBO stays stored in VAO state!)
    glBindVertexArray(0);
}


void ObjectBinders::Bind()
{
	glBindVertexArray(VAOID);
}



void ObjectBinders::ReBind(std::vector<Vertex>& Vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, flatIndices.size() * sizeof(GLuint), flatIndices.data(), GL_DYNAMIC_DRAW);
}

void ObjectBinders::ReBind(std::vector<Vertex>& Vertices, std::vector<Indices>& indices)
{
   

    glBindVertexArray(VAOID);

    glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size() * sizeof(Vertex),Vertices.data(),GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Indices), indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
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

