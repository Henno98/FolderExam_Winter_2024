#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "sstream"
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/ShaderClass.h"
#include "Shaders/ObjectBinders.h"

using namespace std;
class Landscape
{
public:
	
	std::vector<Vertex> vertices;
	std::vector<Vertex> Simplifiedvertices;
	std::vector<Indices> indices;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	float xmin, xmax, zmin, zmax;
	float xoffset, yoffset, zoffset, PrevXval{0};
	float Chunksize{10.f};
	float scale{ 1.f };
	int rows, cols;
	Landscape(const std::string& file, float scale,float chunksize) : Chunksize(chunksize),scale(scale)
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
				vertices.emplace_back(Vertex{ glm::vec3((x-xoffset ),(z-zoffset ),(y -yoffset)),glm::vec3(1.f) });

		}
		cout << "|\n";
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
		//Triangulation();
		processVertices(xmin, xmax, zmin, zmax, Chunksize, scale);
		CalculateNormals();
		initshaders();

	};
	void Triangulation()
	{
		float numpoints{ 0 };
		glm::vec3 vals{ 0.f };
		cout << "|";

		cols = static_cast<int>(ceil((xmax - xmin) / Chunksize)) +1;
		rows = static_cast<int>(ceil((zmax - zmin) / Chunksize)) +1;

		for (int i = 0; i < cols ; i++) {
			float xminrange = xmin + (Chunksize * i);
			float xmaxrange = xmin + (Chunksize * (i + 1));

			

			for (int j = 0; j  < rows  ; j++) {
				float zminrange = zmin + (Chunksize * j);
				float zmaxrange = zmin + (Chunksize * (j + 1));

				vals = glm::vec3(0.f);
				numpoints = 0;
				
			/*	for (const auto& point : vertices) {
					
					if (point.position.x > xminrange && point.position.x < xmaxrange &&
						point.position.z > zminrange && point.position.z < zmaxrange) 
					{
						vals += point.position;
						numpoints++;
						
					}
				}*/
				for (size_t k = 0; k < vertices.size(); ) {
					const auto& point = vertices[k];
					if (point.position.x > xminrange && point.position.x < xmaxrange &&
						point.position.z > zminrange && point.position.z < zmaxrange) {
						vals += point.position;
						++numpoints;

						// Remove vertex by swapping with the last element and popping
						std::swap(vertices[k], vertices.back());
						vertices.pop_back();
					}
					else {
						++k; // Only increment if no removal
					}
				}

				if (numpoints > 0) {
					vals /= numpoints; // Average position
					    // Adjust the position to be the center of the chunk
                glm::vec3 center = glm::vec3(xminrange + (Chunksize / 2), vals.y, zminrange + (Chunksize / 2));
                
                // Store the simplified vertex at the chunk's center
                Simplifiedvertices.emplace_back(center*scale, glm::vec3(1.f,vals.y/10,1.f),glm::vec3(0.f));
				}
				else
				{
					glm::vec3 center = glm::vec3(xminrange + (Chunksize / 2), vals.y , zminrange + (Chunksize / 2));

					// Store the simplified vertex at the chunk's center
					Simplifiedvertices.emplace_back(center*scale, glm::vec3(1.f,vals.y/10,1.f), glm::vec3(0.f));
				}
			}

		}
		// Generate indices for the grid
		for (int i = 0; i < cols-1 ; i++) {
			for (int j = 0; j < rows-1; j++) {
			unsigned int v0 = i * rows + j;
			unsigned int v1 = (i + 1) * rows + j;
			unsigned int v2 = i * rows + (j + 1);
			unsigned int v3 = (i + 1) * rows + (j + 1);           // Bottom-right

			// Make sure indices are within range
			if (v0 < Simplifiedvertices.size() && v1 < Simplifiedvertices.size() &&
				v2 < Simplifiedvertices.size() && v3 < Simplifiedvertices.size()) {
				// First triangle (v0, v1, v2)
				indices.emplace_back(Indices{ v0, v1, v2 });
				// Second triangle (v1, v3, v2)
				indices.emplace_back(Indices{ v1, v3, v2 });
			}
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
	void CalculateNormals()
	{
		float trianglenorm = 0;
		float verticenormal = 0;
		
		for(const auto& triangles : indices)
		{
			const glm::vec3& v0 = Simplifiedvertices[triangles.V0].position;
			const glm::vec3& v1 = Simplifiedvertices[triangles.V1].position;
			const glm::vec3& v2 = Simplifiedvertices[triangles.V2].position;

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;

			glm::vec3 norm = glm::cross(edge1, edge2);

			glm::vec3 normals = glm::normalize(norm);

			Simplifiedvertices[triangles.V0].normal += normals;
			Simplifiedvertices[triangles.V1].normal += normals;
			Simplifiedvertices[triangles.V2].normal += normals;
			trianglenorm++;
			
		}
		for (auto& vertice : Simplifiedvertices)
		{
			vertice.normal = glm::normalize(vertice.normal);
			verticenormal++;
		}
		cout << "calculated normals for: " << trianglenorm << " triangles\n";
		cout << "Calculated normals for : " << verticenormal << " Vertices\n";
	}
	void initshaders()
	{

	
		Binders.Init(Simplifiedvertices);
		Binders.EBOInit(indices);
		Binders.Bind();
		Binders.EBOBind();
		Vertex::BindAttributes();
		Binders.Unbind();
		Binders.EBOUnBind();
		std::cout << "Shaders bound." << endl;
	}
	void SetChunksize(float newsize){
		Chunksize = newsize;
	}
	void SetScale(float newscale)
	{
		scale = newscale;
	}
	void draw(const char* uniform, Shader& shader)
	{


		Binders.Bind();
		Binders.EBOBind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
		//glDrawArrays(GL_POINTS, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size()*3,GL_UNSIGNED_INT,nullptr);
		Binders.Unbind();
		Binders.EBOUnBind();

		

	};
	glm::vec3 Barycentric(glm::vec3& actorPosition, Landscape& terrain)
	{
		for (auto& triangle : terrain.indices) {
			glm::vec3 p1 = Simplifiedvertices[triangle.V0].position;
			glm::vec3 p2 = Simplifiedvertices[triangle.V2].position;
			glm::vec3 p3 = Simplifiedvertices[triangle.V1].position;
			glm::vec3 p4 = actorPosition;
			glm::vec3 NewPosition = actorPosition;

			glm::vec3 p12 = p2 - p1;
			glm::vec3 p13 = p3 - p1;
			glm::vec3 cross = glm::cross(p13, p12);
			float area_123 = cross.y; // double the area
			glm::vec3 baryc; // for return

			// u
			glm::vec3 p = p2 - p4;
			glm::vec3 q = p3 - p4;
			glm::vec3 nu = glm::cross(q, p);
			// double the area of p4pq
			baryc.x = nu.y / area_123;

			// v
			p = p3 - p4;
			q = p1 - p4;
			glm::vec3 nv = glm::cross(q, p);
			// double the area of p4pq
			baryc.y = nv.y / area_123;

			// w
			p = p1 - p4;
			q = p2 - p4;
			glm::vec3 nw = (glm::cross(q, p));
			// double the area of p4pq
			baryc.z = nw.y / area_123;
			if (baryc.x >= 0 && baryc.y >= 0 && baryc.z >= 0 && (baryc.x + baryc.y + baryc.z <= 1)) {

				return baryc.x * p1 + baryc.y * p2 + baryc.z * p3;
				
			}
			return glm::vec3(0.f);
		}
	}
	bool IsInsideTriangle(Landscape& terrain, glm::vec3 actorposition)
	{

		/*glm::vec3 p1 = glm::vec3(Simplifiedvertices[triangle.V0].position.x, 0.0f, Simplifiedvertices[triangle.V0].position.z);
		glm::vec3 p2 = glm::vec3(Simplifiedvertices[triangle.V1].position.x, 0.0f, Simplifiedvertices[triangle.V1].position.z);
		glm::vec3 p3 = glm::vec3(Simplifiedvertices[triangle.V2].position.x, 0.0f, Simplifiedvertices[triangle.V2].position.z);
		glm::vec3 p = glm::vec3(actorposition.x, 0.0f, actorposition.z);*/

		for (auto& triangle : terrain.indices) {
			glm::vec3 p1 = glm::vec3(Simplifiedvertices[triangle.V0].position.x, 0.0f, Simplifiedvertices[triangle.V0].position.z);
		glm::vec3 p2 = glm::vec3(Simplifiedvertices[triangle.V1].position.x, 0.0f, Simplifiedvertices[triangle.V1].position.z);
		glm::vec3 p3 = glm::vec3(Simplifiedvertices[triangle.V2].position.x, 0.0f, Simplifiedvertices[triangle.V2].position.z);
		glm::vec3 p = glm::vec3(actorposition.x, 0.0f, actorposition.z);

			glm::vec3 v0 = p2 - p1;
			glm::vec3 v1 = p3 - p1;
			glm::vec3 v2 = p - p1;

			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);

			float denom = d00 * d11 - d01 * d01;
			if (denom == 0.0f) return false; // Degenerate triangle check

			float u = (d11 * d20 - d01 * d21) / denom;
			float v = (d00 * d21 - d01 * d20) / denom;
			float w = 1.0f - u - v;

			// Check if the point is inside the triangle
			if (u >= 0.0f && v >= 0.0f && w >= 0.0f) {
				return true;
			}
		}
	};


	int getCellIndex(float x, float z, float xmin, float zmin, float Chunksize, int cols) {
		int col = static_cast<int>((x - xmin) / Chunksize);
		int row = static_cast<int>((z - zmin) / Chunksize);
		return row * cols + col; // Flattened index
	}
	void processVertices(float xmin, float xmax, float zmin, float zmax,
		float Chunksize, float scale) {
		// Define grid dimensions
		int cols = static_cast<int>(ceil((xmax - xmin) / Chunksize));
		int rows = static_cast<int>(ceil((zmax - zmin) / Chunksize));

		// Create a grid to hold points in each cell
		std::vector<std::vector<Vertex>> grid(cols * rows);

		// Step 1: Assign points to grid cells
		for (const auto& point : vertices) {
			int cellIndex = getCellIndex(point.position.x, point.position.z, xmin, zmin, Chunksize, cols);
			if (cellIndex >= 0 && cellIndex < grid.size()) {
				grid[cellIndex].push_back(point);
			}
		}

		// Step 2: Process each cell
		for (int i = 0; i < cols; ++i) {
			for (int j = 0; j < rows; ++j) {
				int cellIndex = j * cols + i;

				glm::vec3 vals(0.f);
				int numpoints = grid[cellIndex].size();

				// Aggregate points in the cell
				for (const auto& point : grid[cellIndex]) {
					vals += point.position;
				}

				// Compute the simplified vertex
				glm::vec3 center(xmin + (Chunksize * i) + Chunksize / 2,
					(numpoints > 0) ? vals.y / numpoints : 0.f,
					zmin + (Chunksize * j) + Chunksize / 2);

				// Add to simplified vertices
				Simplifiedvertices.emplace_back(center * scale,
					glm::vec3(1.f, center.y / 10, 1.f),
					glm::vec3(0.f));
			}
		}
		// Generate indices for the grid
		for (int i = 0; i < cols - 1; i++) {
			for (int j = 0; j < rows - 1; j++) {
				unsigned int v0 = i * rows + j;
				unsigned int v1 = (i + 1) * rows + j;
				unsigned int v2 = i * rows + (j + 1);
				unsigned int v3 = (i + 1) * rows + (j + 1);           

				// First triangle (v0, v1, v2)
					indices.emplace_back(Indices{ v0, v1, v2 });
					// Second triangle (v1, v3, v2)
					indices.emplace_back(Indices{ v1, v3, v2 });
				
			}
		}
	}

};

