#pragma once
#include <fstream>

#include <iostream>
#include <string>
#include <vector>
#include "sstream"
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "ObjectBinders.h"

using namespace std;
class Landscape
{
public:
	std::vector<Vertex> vertices;
	std::vector<Vertex> Simplifiedvertices;
	vector<Indices> indices;
	float xmin{ 0 }, xmax, zmin{ 0 }, zmax;
	glm::mat4 Matrix = glm::mat4(1.f);
	float xoffset, yoffset, zoffset;
	ObjectBinders Binders;

	Landscape(const std::string& file)
	{
		ReadFile(file);
	};

	void ReadFile(const std::string& path )
	{
		ifstream file(path);

		if(!file.is_open())
		{

			std::cout << "Failed to load file" << endl;
			return;

		}
		std::string line;
		int linecount = 0;
		if(getline(file,line))
		{
			
			istringstream iss(line);
			
			if (!(iss >> xoffset >> yoffset >> zoffset))
			{

			}
		
		}
		while(std::getline(file,line))
		{
			std::istringstream islinevalid(line);
			float x, y, z,r,g,b;

			if(!(islinevalid >> x >> y >> z >> r >> g >> b))
			{
				continue;

			}
		
			vertices.emplace_back(Vertex{ glm::vec3((x-xoffset),(z-zoffset),(y-yoffset))*scale,glm::vec3(r,g,b)});
				linecount++;
			if(linecount > 10)
			{
				Simplifiedvertices.emplace_back(Vertex{ glm::vec3((x - xoffset),(z - zoffset),(y - yoffset)) * scale,glm::vec3(r,g,b) });

				linecount = 0;
			}
		}

	};

	float scale{ 0.2f };

	void Triangulation()
	{


	}
	
	void initshaders(const char* uniform, Shader& shader)
	{

	
		Binders.Init(reinterpret_cast<GLfloat*>(Simplifiedvertices.data()), (Simplifiedvertices.size() * sizeof(Vertex)));
		Binders.Bind();

		Vertex::BindAttributes();
		

		Binders.Unbind();
	}

	void draw(const char* uniform, Shader& shader)
	{


		Binders.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
		glDrawArrays(GL_POINTS, 0, Simplifiedvertices.size());

		

	};
};

