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
	glm::vec3 P1;
	glm::vec3 P2;
	glm::vec3 P3;
	glm::vec3 P4;
	void Physics(Mesh& actor, float deltatime);
	glm::vec3 Barycentric(glm::vec3& actorPosition,Indices& triangle)
	{

		 P1 = Terrain.Simplifiedvertices[triangle.V0].position;
		P2 = Terrain.Simplifiedvertices[triangle.V2].position;
		P3 = Terrain.Simplifiedvertices[triangle.V1].position;
		P4 = actorPosition;

		BA = P2 - P1;
		CA = P3 - P1;
		glm::vec3 cross = glm::cross(BA, CA);
		float area_123 = cross.y; // double the area
		glm::vec3 baryc; // for return

		// u
		BA = P2 - P4;
		CA = P3 - P4;
		glm::vec3 nu = glm::cross(BA,CA);
		// double the area of p4pq
		baryc.x = nu.y / area_123;

		// v
		BA = P3 - P4;
		CA = P1 - P4;
		glm::vec3 nv = glm::cross(BA,CA);
		// double the area of p4pq
		baryc.y = nv.y / area_123;

		// w
		BA = P1 - P4;
		CA = P2 - P4;
		glm::vec3 nw = (glm::cross(BA,CA));
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
			actor.Velocity *= actor.Friction;
			otheractor.Velocity *= otheractor.Friction;
		}
	}

};
