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
	float xoffset, yoffset, zoffset, PrevXval{0};
	ObjectBinders Binders;
	float Resolution{ 1.f };
	float Chunksize{100.f};
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
		cout << "Inserting Vertices into vector" << endl;
		cout << "|";
		while(std::getline(file,line))
		{
			std::istringstream islinevalid(line);
			float x, y, z;
	
			if(!(islinevalid >> x >> y >> z))
			{
				continue;

			}
			linecount++;
			if (linecount > Resolution)
			{
				vertices.emplace_back(Vertex{ glm::vec3((x-xoffset ),(z-zoffset ),(y -yoffset)) * scale,glm::vec3(1.f) });
				linecount = 0;
			}
			
			
			


				/*if (linecount > 10)
				{
					Simplifiedvertices.emplace_back(Vertex{ glm::vec3((x - xoffset),(z - zoffset),(y - yoffset)) * scale,glm::vec3(r,g,b) });

					linecount = 0;
				}*/
				PrevXval = x;
			
		}
		cout << "|";
		for (auto Points : vertices)
		{
			if (xmax < Points.position.x)
			{
				xmax = Points.position.x;
			}
			if (zmax < Points.position.z)
			{
				zmax = Points.position.z;
			}
			if (xmin > Points.position.x)
			{
				xmin = Points.position.x;
			}
			if (zmin > Points.position.z)
			{
				zmin = Points.position.z;
			}

		}
		cout << "Pointsky Vertices = " << vertices.size() << endl;
		Triangulation();

	};

	float scale{ 1.f };
	int rows, cols;
	void Triangulation()
	{
		float numpoints{ 0 };
		glm::vec3 vals{ 0.f };
		cout << "|";

		cols = static_cast<int>(ceil((xmax - xmin) / Chunksize)) ;
		rows = static_cast<int>(ceil((zmax - zmin) / Chunksize)) ;

		for (int i = 0; i < cols; i++) {
			float xminrange = xmin + (Chunksize * i);
			float xmaxrange = xmin + (Chunksize * (i + 1));

			

			for (int j = 0; j  < rows; j++) {
				float zminrange = zmin + (Chunksize * j);
				float zmaxrange = zmin + (Chunksize * (j + 1));

				vals = glm::vec3(0.f);
				numpoints = 0;
				
				for (const auto& point : vertices) {

					if (point.position.x > xminrange && point.position.x < xmaxrange &&
						point.position.z > zminrange && point.position.z < zmaxrange) 
					{
						vals += point.position;
						numpoints++;
					}
				}

				if (numpoints > 0) {
					vals /= numpoints; // Average position
					Simplifiedvertices.emplace_back(glm::vec3(xmaxrange - (Chunksize / 2), vals.y, zmaxrange - (Chunksize / 2)), glm::vec3(i, 0.f, j));
				}
			}
		}
		// Generate indices for the grid
		for (int i = 0; i < cols -1; i++) {
			for (int j = 0; j < rows-1; j++) {
				unsigned int v0 = i * (rows + 1) + j;         // Top-left
				unsigned int v1 = v0 + 1;                    // Top-right
				unsigned int v2 = (i + 1) * (rows + 1) + j;  // Bottom-left
				unsigned int v3 = v2 + 1;                    // Bottom-right

				// First triangle (v0, v1, v2)
				indices.emplace_back(Indices{ v0, v1, v2 });
				// Second triangle (v1, v3, v2)
				//indices.emplace_back(Indices{ v1, v3, v2 });
			}
		}
		cout << "|" << endl;
		cout << "Triangulated " << Simplifiedvertices.size() << " points" << endl;
		cout << "Triangulated " << indices.size() << " Indices" << endl;
		/*for(auto& Vertex : Simplifiedvertices )
		{
			cout << Vertex.position.x << " " << Vertex.position.y << " " << Vertex.position.z << endl;
		}*/
	}
	
	void initshaders()
	{

	
		Binders.Init(reinterpret_cast<GLfloat*>(Simplifiedvertices.data()), (Simplifiedvertices.size() * sizeof(Vertex)));
		Binders.EBOInit(reinterpret_cast<GLuint*>(indices.data()), (indices.size() * sizeof(Indices)));
		Binders.Bind();
		Binders.EBOBind();
		
		Vertex::BindAttributes();
		

		Binders.Unbind();
		Binders.EBOUnBind();
	}

	void draw(const char* uniform, Shader& shader)
	{


		Binders.Bind();
		Binders.EBOBind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
		
		glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT,nullptr);

		

	};
};

