#include "Functions.h"


void Functions::Physics(Mesh& actor, float deltatime)
{
	accelerationVector = glm::vec3(0.f, Gravity, 0.f);
	if (actor.Position.x > Terrain.xmax || actor.Position.x < Terrain.xmin || actor.Position.z < Terrain.zmin || actor.Position.z > Terrain.zmax) {
		return;
	}
	for (auto& triangle : Terrain.indices)
	{	//check each triangle
		glm::vec3 barycentric = Barycentric(actor.Position, triangle);

		float interpolatedy =
			Terrain.Simplifiedvertices[triangle.V0].position.y * barycentric.x +
			Terrain.Simplifiedvertices[triangle.V2].position.y * barycentric.y +
			Terrain.Simplifiedvertices[triangle.V1].position.y * barycentric.z;

		//if not on plane, continue
		if (actor.Position.y > interpolatedy + actor.Radius) {
			continue;
		}
			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && (barycentric.x + barycentric.y + barycentric.z <= 1)) {

				actor.Position.y = interpolatedy + actor.Radius;
				accelerationVector = glm::vec3(0.f);


				P1 = Terrain.Simplifiedvertices[triangle.V0].position;
				P2 = Terrain.Simplifiedvertices[triangle.V2].position;
				P3 = Terrain.Simplifiedvertices[triangle.V1].position;

				BA = P2 - P1;
				CA = P3 - P1;
				normal = glm::cross(BA, CA);
				normalized = glm::normalize(normal);

				//old code using class notes
				//glm::vec3 norm = glm::vec3(normalized.x * normalized.z, normalized.y * normalized.z, (normalized.z * normalized.z) - 1.f);
				//glm::vec3 normal = terrain.Normals;

				//new code accounting for slope steepness and angle
				  // Project velocity onto the slope's tangent plane
				glm::vec3 velocityParallelToSlope = actor.Velocity - glm::dot(actor.Velocity, normalized) * normalized;

				// Apply gravity along the slope
				glm::vec3 gravityAlongSlope = gravity - glm::dot(gravity, normalized) * normalized;

				// Update velocity and acceleration
				actor.Velocity = velocityParallelToSlope + gravityAlongSlope * deltatime;
				// Update sphere dynamics
				actor.Velocity.x *= actor.Friction; // Example friction factor
				actor.Velocity.z *= actor.Friction;
				break;
			}


		
	}
		actor.Acceleration = accelerationVector;
		actor.Velocity += (actor.Acceleration * deltatime);
		actor.Position += (actor.Velocity * deltatime);
		actor.collider.Position = actor.Position;
		actor.Matrix = actor.MatrixCalc();
	
	
}
