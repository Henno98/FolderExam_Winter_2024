#pragma once
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/ShaderClass.h"
#include "Shaders/ObjectBinders.h"


struct Chunk
{
public:
	std::vector<Vertex> vertices;
	std::vector<Indices> indices;
	glm::vec3 position;
	float xmin, xmax, zmin, zmax;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	int CurrentLOD{ 0 };
	int TargetLOD{ 0 };
	int indexCount{ 0 };
	glm::vec3 center;

	void ComputeCenter()
	{
		center = {
			(xmin + xmax) * 0.5f,
			0.0f,
			(zmin + zmax) * 0.5f
		};
	}

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


		Binders.Init(vertices, indices);
		Binders.Bind();
		Binders.EBOUnBind();
		Binders.Unbind();

	}

	//std::vector<Indices> Delaunay(std::vector<Vertex>& verts)
	//{

	//	std::vector<Indices> tris;
	//	tris.reserve(verts.size() * 3);

	//	// Super triangle
	//	for (const auto& v : verts)
	//	{
	//		xmin = std::min(xmin, v.position.x);
	//		xmax = std::max(xmax, v.position.x);
	//		zmin = std::min(zmin, v.position.z);
	//		zmax = std::max(zmax, v.position.z);
	//	}

	//	float dx = xmax - xmin;
	//	float dz = zmax - zmin;
	//	float delta = std::max(dx, dz) * 10.0f;

	//	float midx = (xmin + xmax) * 0.5f;
	//	float midz = (zmin + zmax) * 0.5f;
	//	unsigned int s0 = verts.size();

	//	verts.emplace_back(Vertex{ { midx - delta, 0, midz - delta } });
	//	verts.emplace_back(Vertex{ { midx + delta, 0, midz - delta } });
	//	verts.emplace_back(Vertex{ { midx,         0, midz + delta } });

	//	tris.push_back({ s0, s0 + 1, s0 + 2 });

	//	std::vector<Indices> newTris;
	//	newTris.reserve(tris.capacity());
	//	std::vector<char> isBad;
	//		isBad.reserve(verts.size());
	//	// --- main loop ---
	//	for (unsigned int i = 0; i < s0; i++)
	//	{
	//		const Vertex& p = verts[i];

	//		isBad.assign(tris.size(), false);
	//		std::vector<int> bad;
	//		bad.reserve(tris.size());

	//		
	//		//find bad triangles
	//	
	//		for (int t = 0; t < (int)tris.size(); t++)
	//		{
	//			const auto& tri = tris[t];

	//			const auto& v0 = verts[tri.V0].position;
	//			const auto& v1 = verts[tri.V1].position;
	//			const auto& v2 = verts[tri.V2].position;

	//			float minX = std::min({ v0.x, v1.x, v2.x });
	//			float maxX = std::max({ v0.x, v1.x, v2.x });
	//			float minZ = std::min({ v0.z, v1.z, v2.z });
	//			float maxZ = std::max({ v0.z, v1.z, v2.z });

	//			if (p.position.x < minX || p.position.x > maxX ||
	//				p.position.z < minZ || p.position.z > maxZ)
	//				continue;

	//			if (inCircumcircle(
	//				verts[tri.V0],
	//				verts[tri.V1],
	//				verts[tri.V2],
	//				p))
	//			{
	//				isBad[t] = true;
	//				bad.emplace_back(t);
	//			}
	//		}

	//		
	//		// build boundary edges
	//	



	//		std::unordered_set<EdgeKey, EdgeHash> boundary;

	//		auto addEdge = [&](unsigned int a, unsigned int b)
	//			{
	//				EdgeKey e{ a, b };

	//				if (boundary.find(e) != boundary.end())
	//					boundary.erase(e);
	//				else
	//					boundary.insert(e);
	//			};

	//		for (int idx : bad)
	//		{
	//			const auto& t = tris[idx];

	//			addEdge(t.V0, t.V1);
	//			addEdge(t.V1, t.V2);
	//			addEdge(t.V2, t.V0);
	//		}

	//		
	//		// compact triangles
	//	
	//		newTris.clear();

	//		for (int t = 0; t < (int)tris.size(); t++)
	//		{
	//			if (!isBad[t])
	//				newTris.emplace_back(tris[t]);
	//		}

	//		tris.swap(newTris);

	//	
	//		// re-triangulate cavity
	//		
	//		for (const auto& e : boundary)
	//		{
	//			tris.emplace_back(Indices{ e.a, e.b, i });
	//		}


	//	}

	//	//remove super triangle
	//	std::vector<Indices> finalTris;
	//	finalTris.reserve(tris.size());

	//	for (const auto& t : tris) {
	//		if (t.V0 < s0 && t.V1 < s0 && t.V2 < s0)
	//		{
	//			finalTris.emplace_back(t);
	//			
	//		}
	//	}
	//	
	//	verts.resize(s0);
	//	return finalTris;

	//}
	//float dist2(const Vertex& a, const Vertex& b) {
	//	float dx = a.position.x - b.position.x;
	//	float dz = a.position.z - b.position.z;
	//	return dx * dx + dz * dz;
	//}

	//// signed area
	//float orient(const Vertex& a, const Vertex& b, const Vertex& c) {
	//	return (b.position.x - a.position.x) * (c.position.z - a.position.z) - (b.position.z - a.position.z) * (c.position.x - a.position.x);
	//}
	//// circumcircle test 
	//bool inCircumcircle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& p) {

	//	float ax = a.position.x - p.position.x;
	//	float az = a.position.z - p.position.z;
	//	float bx = b.position.x - p.position.x;
	//	float bz = b.position.z - p.position.z;
	//	float cx = c.position.x - p.position.x;
	//	float cz = c.position.z - p.position.z;
	//	float det =
	//		(ax * ax + az * az) * (bx * cz - bz * cx) -
	//		(bx * bx + bz * bz) * (ax * cz - az * cx) +
	//		(cx * cx + cz * cz) * (ax * bz - az * bx);
	//	return det > 0;
	//}
	//// edge representation 
	//struct EdgeKey {
	//	unsigned int a, b;
	//	bool operator==(const EdgeKey& o) const {
	//		return a == o.a && b == o.b;
	//	}
	//};
	///*bool sameEdge(const EdgeKey& e1, const EdgeKey& e2) {
	//	return (e1.a == e2.b && e1.b == e2.a) ||
	//		(e1.a == e2.a && e1.b == e2.b);
	//}*/
	//struct EdgeHash
	//{
	//	size_t operator()(const EdgeKey& e) const
	//	{
	//		return std::hash<unsigned int>()(std::min(e.a, e.b)) ^
	//			std::hash<unsigned int>()(std::max(e.a, e.b));
	//	}
	//};
	//bool sameEdge(const EdgeKey& e1, const EdgeKey& e2)
	//{
	//	return (e1.a == e2.a && e1.b == e2.b) ||
	//		(e1.a == e2.b && e1.b == e2.a);
	//}
	void Clear()
	{
		vertices.clear();
		indices.clear();
	}

	

};

using namespace std;
class Landscape
{
public:
	std::vector<Chunk> Chunks;
	
	int ChunkSize{ 1 };

	std::vector<Vertex> vertices;
	std::vector<Vertex> Simplifiedvertices;
	std::vector<Indices> indices;
	ObjectBinders Binders;
	glm::mat4 Matrix = glm::mat4(1.f);
	float xmin, xmax, zmin, zmax;
	float xoffset, yoffset, zoffset, PrevXval{0};
	float LOD{1.f};
	float scale{ 1.f };
	int rows, cols;

	Chunk* GetChunk(glm::vec3 pos)
	{
		for (auto& chunk : Chunks)
		{
			if (pos.x >= chunk.xmin && pos.x <= chunk.xmax &&
				pos.z >= chunk.zmin && pos.z <= chunk.zmax)
			{
				return &chunk;
			}
		}
		return nullptr;
	}

	Landscape() = default;

	Landscape(const std::string& file)
	{
		ReadFile(file);
	}
	Landscape(const std::string& file, float scale) : scale(scale)
	{
		ReadFile(file);
	}
	Landscape(const std::string& file, float scale, float LOD) : LOD(LOD), scale(scale)
	{
		ReadFile(file);
	}
	Landscape(const std::string& file, float scale, float LOD, int chunkSize) : LOD(LOD), scale(scale), ChunkSize(chunkSize)
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

	
	}

		
	void TriangulateChunk(Chunk& chunk)
	{
		//if (chunk.vertices.size() < 3)
		//	return;

		//std::vector<double> coords;
		//coords.reserve(chunk.vertices.size() * 2);

		//// 1. flatten to 2D 
		//for (const auto& v : chunk.vertices)
		//{
		//	coords.push_back((double)v.position.x);
		//	coords.push_back((double)v.position.z);
		//}

		//// 2. run delaunay
		//delaunator::Delaunator d(coords);

		//// 3. build indices
		//chunk.indices.clear();
		//chunk.indices.reserve(d.triangles.size() / 3);

		//for (size_t i = 0; i < d.triangles.size(); i += 3)
		//{
		//	chunk.indices.emplace_back(Indices{
		//		(unsigned int)d.triangles[i],
		//		(unsigned int)d.triangles[i + 1],
		//		(unsigned int)d.triangles[i + 2]
		//		});
		//}
	}
		
	

		void SimplifyVertices()
		{
			if (LOD <= 0.0f)
			{
				cout << "Invalid LOD for SimplifyVertices\n";
				return;
			}

			// Determine grid resolution
			int cols = static_cast<int>(ceil((xmax - xmin) / LOD));
			int rows = static_cast<int>(ceil((zmax - zmin) / LOD));
			if (cols <= 0 || rows <= 0)
			{
				cout << "No grid cells for SimplifyVertices\n";
				return;
			}

			// Accumulators for averaging Y per cell
			std::vector<glm::vec3> sum(cols * rows, glm::vec3(0.0f));
			std::vector<int> count(cols * rows, 0);

			// Accumulate positions into grid cells
			for (const auto& v : vertices)
			{
				int gx = static_cast<int>((v.position.x - xmin) / ChunkSize);
				int gz = static_cast<int>((v.position.z - zmin) / ChunkSize);

				if (gx < 0 || gx >= cols || gz < 0 || gz >= rows)
					continue;

				int idx = gz * cols + gx;
				sum[idx] += v.position;
				count[idx] += 1;
			}

			// Build simplified vertex list
			Simplifiedvertices.clear();
			Simplifiedvertices.reserve(cols * rows);

			for (int gz = 0; gz < rows; ++gz)
			{
				for (int gx = 0; gx < cols; ++gx)
				{
					int idx = gz * cols + gx;
					float y = 0.0f;
					if (count[idx] > 0)
						y = sum[idx].y / count[idx];

					float cx = xmin + gx * LOD + LOD / 2;
					float cz = zmin + gz * LOD + LOD / 2;

					Simplifiedvertices.emplace_back(Vertex{ glm::vec3(cx, y, cz), glm::vec3(1.f), glm::vec3(1.f) });
				}
			}
			cout << "Simplified Vertices: " << Simplifiedvertices.size() << "\n";
		}

		void Render(Shader& shader, const char* uniform)
		{
			for (auto& chunk : Chunks)
			{
				chunk.Binders.Bind();
				glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(chunk.Matrix));
				glDrawElements(GL_TRIANGLES,chunk.indices.size() * 3,GL_UNSIGNED_INT,0);
			}
		}


		void InitLandscape()
	{
			ProcessChunkTriangles(xmin, xmax, zmin, zmax, ChunkSize, vertices);
			#pragma omp parallel for
			for (int i = 0; i < Chunks.size(); i++)
			{
				
			}
			int i = 0;
			for (auto& chunk : Chunks)
			{
				i++;
				auto tt = std::chrono::high_resolution_clock::now();
				TriangulateChunk(chunk);
				auto tz = std::chrono::high_resolution_clock::now();
				std::cout << "Chunk indices initialized in: " << std::chrono::duration_cast<std::chrono::milliseconds>(tz - tt).count() << " ms\n";
				std::cout << "Chunk " << i << " has " << chunk.indices.size() << " triangles\n";
				CalculateNormals(chunk.indices, chunk.vertices);
				chunk.InitChunkShaders();
				//std::cout << "Chunk indices initialized in: " << std::chrono::duration_cast<std::chrono::milliseconds>(tz - tt).count() << " ms\n";
			}

		
	}


	
	void CalculateNormals(std::vector<Indices>& indices, std::vector<Vertex>& vertices)
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

	void initshaders(std::vector<Vertex>& vertices, std::vector<Indices>& indices)
	{
		


		Binders.Init(vertices, indices);
		Binders.Bind();
		Binders.Unbind();
		std::cout << "Shaders bound." << endl;
	}

	void DeleteShaders()
	{
		Binders.Delete();
	}

	void SetChunksize(float newsize)
	{
		ChunkSize = newsize;
	}

	void SetScale(float newscale)
	{
		scale = newscale;
	}

	void Draw(const char* uniform, Shader& shader)
	{
		

		Binders.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Matrix));
		//glDrawArrays(GL_POINTS, 0, vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, nullptr);
		
		
		

	}

	int getCellIndex(float x, float z, float xmin, float zmin, float Chunksize, int cols) 
	{
		int col = static_cast<int>((x - xmin) / Chunksize);
		int row = static_cast<int>((z - zmin) / Chunksize);
		return row * cols + col; // Flattened index
	}


	void ProcessVertices(float xmin, float xmax, float zmin, float zmax,float LOD, float scale) 
	{
		// Define grid dimensions
		int cols = static_cast<int>(ceil((xmax - xmin) / LOD));
		int rows = static_cast<int>(ceil((zmax - zmin) / LOD));

		// Create a grid to hold points in each cell
		std::vector<std::vector<Vertex>> grid(cols * rows);

		// Step 1: Assign points to grid cells
		for (const auto& point : vertices) {
			int cellIndex = getCellIndex(point.position.x, point.position.z, xmin, zmin, LOD, cols);
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
				glm::vec3 center(xmin + (LOD * i) + LOD / 2,
					yCoordinate,
					zmin + (LOD * j) + LOD / 2);

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
	void ProcessChunkTriangles(
		float xmin, float xmax,
		float zmin, float zmax,
		int chunkSize,
		const std::vector<Vertex>& vertices
		)
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

			Chunks[idx].vertices.emplace_back(verts.position, glm::vec3(1.f, abs(verts.position.y) / 100, 1.f));
		}

		std::cout << "Finished processing triangles into chunks" << "\n";
		std::cout << "Chunk count: " << Chunks.size() << "\n";

		
		
	}



		
	
};

