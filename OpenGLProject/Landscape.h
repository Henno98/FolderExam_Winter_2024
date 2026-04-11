#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/ShaderClass.h"

#include "Shaders/ObjectBinders.h"

struct Chunk
{
	std::vector<Vertex> vertices;
	std::vector<Indices> indices;
	float xmin, xmax, zmin, zmax;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	std::unordered_map<int, unsigned int> gridToLocal;
	int gridPerChunk = 0;

	void DeleteChunkBinders()
	{
	
			Binders.Delete();
		
	}

	void InitChunkShaders()
	{
		if (vertices.empty())
		{
			return;
		}
		
		
			Binders.Init(vertices);
			Binders.EBOInit(indices);
			Binders.Bind();
			Binders.EBOBind();
			Vertex::BindAttributes();
			Binders.EBOUnBind();
			Binders.Unbind();


		
	}


	void CalculateChunkIndices(int Chunksize) {
		
		indices = Delaunay(vertices);
	}

	std::vector<Indices> Delaunay(std::vector<Vertex>& verts)
	{
		std::vector<Indices> tris;
		tris.reserve(verts.size() * 3);

		// --- Super triangle ---
		unsigned int s0 = verts.size();

		verts.push_back({ -10000, 0, -10000 });
		verts.push_back({ 10000, 0, -10000 });
		verts.push_back({ 0,     0,  10000 });

		tris.push_back({ s0, s0 + 1, s0 + 2 });

		std::vector<Indices> newTris;
		newTris.reserve(tris.capacity());

		// --- main loop ---
		for (unsigned int i = 0; i < s0; i++)
		{
			const Vertex& p = verts[i];

			std::vector<char> isBad(tris.size(), false);
			std::vector<int> bad;
			bad.reserve(tris.size());

			// ----------------------------
			// 1. find bad triangles
			// ----------------------------
			for (int t = 0; t < (int)tris.size(); t++)
			{
				const auto& tri = tris[t];

				if (inCircumcircle(
					verts[tri.V0],
					verts[tri.V1],
					verts[tri.V2],
					p))
				{
					isBad[t] = true;
					bad.push_back(t);
				}
			}

			// ----------------------------
			// 2. build boundary edges
			// (fast O(k²) but small k)
			// ----------------------------
		

			
			std::vector<EdgeKey> boundary;
			boundary.reserve(bad.size() * 2);

			auto addEdge = [&](unsigned int a, unsigned int b)
				{
					EdgeKey e{ a, b };

					for (auto it = boundary.begin(); it != boundary.end(); ++it)
					{
						if (sameEdge(*it, e))
						{
							boundary.erase(it);
							return;
						}
					}

					boundary.push_back(e);
				};

			for (int idx : bad)
			{
				const auto& t = tris[idx];

				addEdge(t.V0, t.V1);
				addEdge(t.V1, t.V2);
				addEdge(t.V2, t.V0);
			}

			// ----------------------------
			// 3. compact triangles (FAST)
			// ----------------------------
			newTris.clear();

			for (int t = 0; t < (int)tris.size(); t++)
			{
				if (!isBad[t])
					newTris.push_back(tris[t]);
			}

			tris.swap(newTris);

			// ----------------------------
			// 4. re-triangulate cavity
			// ----------------------------
			for (const auto& e : boundary)
			{
				tris.push_back({ e.a, e.b, i });
			}

			
		}

		// --- remove super triangle ---
		std::vector<Indices> finalTris;
		finalTris.reserve(tris.size());

		for (const auto& t : tris)
		{
			if (t.V0 < s0 && t.V1 < s0 && t.V2 < s0)
				finalTris.push_back(t);
		}

		verts.resize(s0);



		return finalTris;
	}

	float dist2(const Vertex& a, const Vertex& b) {
		float dx = a.position.x - b.position.x;
		float dz = a.position.z - b.position.z;
		return dx * dx + dz * dz;
	}
	
	// signed area (orientation test)
	float orient(const Vertex& a, const Vertex& b, const Vertex& c) {
		return (b.position.x - a.position.x) * (c.position.z - a.position.z) - (b.position.z - a.position.z) * (c.position.x - a.position.x);
	}
	// circumcircle test (returns true if p is inside triangle abc)
	bool inCircumcircle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& p) {
		
		float ax = a.position.x - p.position.x;
		float az = a.position.z - p.position.z;
		float bx = b.position.x - p.position.x;
		float bz = b.position.z - p.position.z;
		float cx = c.position.x - p.position.x;
		float cz = c.position.z - p.position.z;
		float det =
			(ax * ax + az * az) * (bx * cz - bz * cx) -
			(bx * bx + bz * bz) * (ax * cz - az * cx) +
			(cx * cx + cz * cz) * (ax * bz - az * bx);
		return det > 0;
	}
	// edge representation (for polygon hole boundary)
	struct EdgeKey {
		unsigned int a, b;
		bool operator==(const EdgeKey& o) const {
			return a == o.a && b == o.b;
		}
	};
	bool sameEdge(const EdgeKey& e1, const EdgeKey& e2) {
		return (e1.a == e2.b && e1.b == e2.a) ||
			(e1.a == e2.a && e1.b == e2.b);
	}
	void DrawChunks(Shader& shader, const char* uniform)
	{
		if (vertices.empty())
		{
			std::cout << "Failed to bind shader, vertices empty\n";
			return;
		}
		

			Binders.Bind();
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
			Binders.EBOBind();
			glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, nullptr);
			glDrawArrays(GL_POINTS, 0, vertices.size());
			Binders.Unbind();
			Binders.EBOUnBind();
		
	}

};

using namespace std;
class Landscape
{
public:
	
	std::vector<Chunk> Chunks;
	int ChunkSize{ 10 };

	void ProcessChunkTriangles(float xmin, float xmax, float zmin, float zmax, float chunkSize, const std::vector<Vertex>& vertices)
	{
		int cols = static_cast<int>(ceil((xmax - xmin) / chunkSize));
		int rows = static_cast<int>(ceil((zmax - zmin) / chunkSize));

		Chunks.resize(cols * rows);

		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				int idx = i + j * cols;

				Chunks[idx].xmin = xmin + i * chunkSize;
				Chunks[idx].xmax = Chunks[idx].xmin + chunkSize;

				Chunks[idx].zmin = zmin + j * chunkSize;
				Chunks[idx].zmax = Chunks[idx].zmin + chunkSize;
			}
		}


		for (const auto& verts : vertices)
		{
			int cx = static_cast<int>((verts.position.x - xmin) / chunkSize);
			int cz = static_cast<int>((verts.position.z - zmin) / chunkSize);

			if (cx < 0 || cx >= cols || cz < 0 || cz >= rows)
				continue;

			int idx = cx + cz * cols;

			Chunks[idx].vertices.emplace_back(verts.position,glm::vec3(1.f,abs(verts.position.y)/100,1.f));
		}
	

		std::cout << "Finished processing triangles into chunks" << "\n";
		std::cout << "Chunk count: " << Chunks.size() << "\n";
		for (auto& chunk : Chunks) {

			

			std::cout << "Chunk vertices: " << chunk.vertices.size() << "\n";
		}

	}

	void DrawChunks(Shader& shader, const char* str)
	{
		
		for (auto& chunk : Chunks)
		{
			chunk.DrawChunks(shader, str);
		}

	}


	std::vector<Vertex> vertices;
	std::vector<Vertex> Simplifiedvertices;
	std::vector<Indices> indices;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	float xmin, xmax, zmin, zmax;
	float xoffset, yoffset, zoffset, PrevXval{0};
	float LOD{10.f};
	float scale{ 1.f };
	int rows, cols;


	Landscape(const std::string& file, float scale,float LOD) : LOD(LOD),scale(scale)
	{
		ReadFile(file);
	}
	void ReadFile(const std::string& path )
	{
		ifstream file(path);

	
		if(!file.is_open())
		{

			std::cout << "Failed to load file\n";
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
		cout << "Inserting Vertices into vector\n";
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
		for (const auto& v : vertices)
		{
			xmin = std::min(xmin, v.position.x);
			xmax = std::max(xmax, v.position.x);
			zmin = std::min(zmin, v.position.z);
			zmax = std::max(zmax, v.position.z);
		}
		cout << "Pointsky Vertices = " << vertices.size() << "\n";
		file.close();

		//processVertices(xmin, xmax, zmin, zmax, LOD, scale);
		//CalculateNormals();
		//initshaders();


		ProcessChunkTriangles(xmin, xmax, zmin, zmax, ChunkSize, vertices);
		for (auto& chunk : Chunks) {

			chunk.CalculateChunkIndices(ChunkSize);
			CalculateNormals(chunk.indices, chunk.vertices);
			std::cout <<"indices: " << chunk.indices.size() << "\n";
		}
		for (int i = 0; i < Chunks.size();i++)
		{
			Chunks[i].InitChunkShaders();
		}
		std::cout << "Finished processing vertices\n";

	}
	//void Triangulation(std::vector<Vertex>& vertices, std::vector<Indices>& indices)
	//{
	//	float numpoints{ 0 };
	//	glm::vec3 vals{ 0.f };
	//	cout << "|";

	//	cols = static_cast<int>(ceil((xmax - xmin) / LOD)) +1;
	//	rows = static_cast<int>(ceil((zmax - zmin) / LOD)) +1;

	//	for (int i = 0; i < cols ; i++) {
	//		float xminrange = xmin + (LOD * i);
	//		float xmaxrange = xmin + (LOD * (i + 1));

	//		

	//		for (int j = 0; j  < rows  ; j++) {
	//			float zminrange = zmin + (LOD * j);
	//			float zmaxrange = zmin + (LOD * (j + 1));

	//			vals = glm::vec3(0.f);
	//			numpoints = 0;
	//		


	//			for (size_t k = 0; k < vertices.size(); ) {
	//				const auto& point = vertices[k];
	//				if (point.position.x > xminrange && point.position.x < xmaxrange &&
	//					point.position.z > zminrange && point.position.z < zmaxrange) {
	//					vals += point.position;
	//					++numpoints;

	//					// Remove vertex by swapping with the last element and popping
	//					std::swap(vertices[k], vertices.back());
	//					vertices.pop_back();
	//				}
	//				else {
	//					++k; // Only increment if no removal
	//				}
	//			}

	//			if (numpoints > 0) {
	//				vals /= numpoints; // Average position
	//				    // Adjust the position to be the center of the chunk
 //               glm::vec3 center = glm::vec3(xminrange + (LOD / 2), vals.y, zminrange + (LOD / 2));
 //               
 //               // Store the simplified vertex at the chunk's center
 //               Simplifiedvertices.emplace_back(center*scale, glm::vec3(1.f,vals.y/10,1.f),glm::vec3(0.f));
	//			}
	//			else
	//			{
	//				glm::vec3 center = glm::vec3(xminrange + (LOD / 2), vals.y , zminrange + (LOD / 2));

	//				// Store the simplified vertex at the chunk's center
	//				Simplifiedvertices.emplace_back(center*scale, glm::vec3(1.f,vals.y/10,1.f), glm::vec3(0.f));
	//			}
	//		}

	//	}
	//	// Generate indices for the grid
	//	for (int i = 0; i < cols-1 ; i++) {
	//		for (int j = 0; j < rows-1; j++) {
	//		unsigned int v0 = i * rows + j;
	//		unsigned int v1 = (i + 1) * rows + j;
	//		unsigned int v2 = i * rows + (j + 1);
	//		unsigned int v3 = (i + 1) * rows + (j + 1);           // Bottom-right

	//		// Make sure indices are within range
	//		if (v0 < Simplifiedvertices.size() && v1 < Simplifiedvertices.size() &&
	//			v2 < Simplifiedvertices.size() && v3 < Simplifiedvertices.size()) {
	//			// First triangle (v0, v1, v2)
	//			indices.emplace_back(Indices{ v0, v1, v2 });
	//			// Second triangle (v1, v3, v2)
	//			indices.emplace_back(Indices{ v1, v3, v2 });
	//		}
	//		}
	//	}
	//	cout << "|" << endl;
	//	cout << "Triangulated " << Simplifiedvertices.size() << " points" << endl;
	//	cout << "Triangulated " << indices.size() << " Indices" << endl;
	//	/*for(auto& Vertex : Simplifiedvertices )
	//	{
	//		cout << Vertex.position.x << " " << Vertex.position.y << " " << Vertex.position.z << endl;
	//	}*/
	//}
	void CalculateNormals(std::vector<Indices>& indices, std::vector<Vertex> vertices)
	{
		float trianglenorm = 0;
		float verticenormal = 0;
		
		for(const auto& triangles : indices)
		{
			const glm::vec3& v0 = vertices[triangles.V0].position;
			const glm::vec3& v1 = vertices[triangles.V1].position;
			const glm::vec3& v2 = vertices[triangles.V2].position;

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;

			glm::vec3 norm = glm::cross(edge1, edge2);

			glm::vec3 normals = glm::normalize(norm);
			

			vertices[triangles.V0].normal += normals;
			vertices[triangles.V1].normal += normals;
			vertices[triangles.V2].normal += normals;
			trianglenorm++;
			vertices[triangles.V0].Color += normals;
			vertices[triangles.V1].Color += normals;
			vertices[triangles.V2].Color += normals;
			
		}
		for (auto& vertice : vertices)
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
		LOD = newsize;
	}
	void SetScale(float newscale)
	{
		scale = newscale;
	}
	void Draw(const char* uniform, Shader& shader)
	{


		Binders.Bind();
		Binders.EBOBind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
		//glDrawArrays(GL_POINTS, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size()*3,GL_UNSIGNED_INT,nullptr);
		Binders.Unbind();
		Binders.EBOUnBind();

		

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

				float yCoordinate;
				if (numpoints > 0) {
					yCoordinate = vals.y / numpoints;
				}
				else {
					yCoordinate = 0.f;
				}
				glm::vec3 center(xmin + (Chunksize * i) + Chunksize / 2,
					yCoordinate,
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

