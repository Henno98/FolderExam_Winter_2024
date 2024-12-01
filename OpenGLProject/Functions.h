#pragma once
#include <fstream>
#include <iostream>
#include <vector>

#include "Landscape.h"
#include "Mesh.h"
#include "sstream"
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "Shaders/ShaderClass.h"
#include "Shaders/ObjectBinders.h"
class Functions
{
public:
	Landscape Terrain;
	Functions(Landscape& terrain) : Terrain(terrain){}
	float Gravity = -9.81f;
	glm::vec3 gravity = glm::vec3(0, -9.81f, 0);
	glm::vec3 accelerationVector = glm::vec3(0.f);
	glm::vec3 BA;
	glm::vec3 CA;
	glm::vec3 normal;
	glm::vec3 normalized;
	void Physics(Mesh& actor, float deltatime);
	glm::vec3 Barycentric(glm::vec3& actorPosition, Landscape& chunk,Indices& triangle)
	{

		glm::vec3 p1 = chunk.Simplifiedvertices[triangle.V0].position;
		glm::vec3 p2 = chunk.Simplifiedvertices[triangle.V2].position;
		glm::vec3 p3 = chunk.Simplifiedvertices[triangle.V1].position;
		glm::vec3 p4 = actorPosition;

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

		return baryc;


	}
	void Collision(Mesh& otheractor, std::vector<Mesh> actor)
	{
		for (int i = 0; i < actor.size(); i++) {

			glm::vec3 min = otheractor.collider.Position - otheractor.collider.Extent;
			glm::vec3 max = otheractor.collider.Position + otheractor.collider.Extent;
			glm::vec3 spheremin = actor[i].Position;
			glm::vec3 spheremax = otheractor.Position;
			glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
			float diameter = glm::distance(spheremin, spheremax);
			// Calculate the vector between sphere centers and distance
			glm::vec3 distanceVector = actor[i].Position - otheractor.Position;
			float distance = glm::length(distanceVector);
			float combinedRadius = actor[i].Radius + otheractor.Radius;
			if (diameter <= actor[i].Radius * 2 && distance > 0) {

				// Resolve overlap by moving spheres apart
				float penetrationDepth = combinedRadius - distance;
				glm::vec3 collisionNormal = glm::normalize(distanceVector);

				// Move spheres proportional to their masses
				glm::vec3 separation = collisionNormal * penetrationDepth;
				actor[i].Position += separation * (otheractor.Mass / (actor[i].Mass + otheractor.Mass));
				otheractor.Position -= separation * (actor[i].Mass / (actor[i].Mass + otheractor.Mass));

				glm::vec3 V1 = ((actor[i].Mass - otheractor.Mass) / (actor[i].Mass + otheractor.Mass) * actor[i].Velocity)
					+ ((2 * otheractor.Mass) / (actor[i].Mass + otheractor.Mass) * otheractor.Velocity);
				glm::vec3 V2 = ((actor[i].Mass * 2) / (actor[i].Mass + otheractor.Mass) * actor[i].Velocity)
					+ ((otheractor.Mass - actor[i].Mass) / (actor[i].Mass + otheractor.Mass) * otheractor.Velocity);
				actor[i].Velocity = V1;
				otheractor.Velocity = V2;
				cout << "Ball collided with ball " << i << endl;
			}
		}
	}

};
