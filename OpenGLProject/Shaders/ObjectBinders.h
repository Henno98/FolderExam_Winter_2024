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
	
	void Init(std::vector<Vertex>& Vertices, std::vector<Indices>& indices);

	//void EBOInit(std::vector<Indices>& indices);

	void Bind();

	

	void ReBind(std::vector<Vertex>& Vertices);
	void ReBind(std::vector<Vertex>& Vertices, std::vector<Indices>& indices);

	

	void EBOUnBind();

	void Unbind();

	void Delete();

	std::vector<GLuint> FlattenIndices(std::vector<Indices>& indices)
	{
		std::vector<GLuint> flatIndices;
		flatIndices.reserve(indices.size() * 3);

		for (auto& tri : indices)
		{
			flatIndices.push_back(tri.V0);
			flatIndices.push_back(tri.V1);
			flatIndices.push_back(tri.V2);
		}


		return flatIndices;
	};

};

