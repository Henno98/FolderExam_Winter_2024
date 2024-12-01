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
	void Collision(Mesh& actor, Mesh& otheractor) {
		// Calculate the vector between the centers of the two objects
		glm::vec3 distanceVector = actor.Position - otheractor.Position;
		float distance = glm::length(distanceVector);

		// Calculate combined radii of the two objects
		float combinedRadius = actor.Radius + otheractor.Radius;

		// If distance is less than combined radii, there is a collision
		if (distance < combinedRadius && distance > 0) {
			// Normalize the collision normal (unit vector in the direction of separation)
			glm::vec3 collisionNormal = glm::normalize(distanceVector);

			// Calculate the penetration depth
			float penetrationDepth = combinedRadius - distance;

			// Resolve the overlap by moving the objects apart proportional to their masses
			glm::vec3 separation = collisionNormal * (penetrationDepth * 0.5f); // Split separation equally
			actor.Position += separation * (otheractor.Mass / (actor.Mass + otheractor.Mass));
			otheractor.Position -= separation * (actor.Mass / (actor.Mass + otheractor.Mass));

			// Exchange velocities based on the masses of the objects (elastic collision)
			glm::vec3 V1 = ((actor.Mass - otheractor.Mass) / (actor.Mass + otheractor.Mass) * actor.Velocity)
				+ ((2 * otheractor.Mass) / (actor.Mass + otheractor.Mass) * otheractor.Velocity);
			glm::vec3 V2 = ((otheractor.Mass - actor.Mass) / (actor.Mass + otheractor.Mass) * otheractor.Velocity)
				+ ((2 * actor.Mass) / (actor.Mass + otheractor.Mass) * actor.Velocity);

			actor.Velocity = V1;
			otheractor.Velocity = V2;

			// Apply damping to the velocities to avoid infinite bouncing
			actor.Velocity *= 0.98f;
			otheractor.Velocity *= 0.98f;
		}
	}

};

