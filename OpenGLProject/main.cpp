#include <iostream>
#include <fstream>
#include <iomanip>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstddef>
#include <string>
#include <Eigen/Dense>
#include <threads.h>

#include "Bezier.h"
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
#include "Functions.h"
#include "Shaders/Light.h"
#include "Landscape.h"
#include "Models/Cube.h"
#include "Models/NPC.h"
#include "Models/Trophy.h"
#include "Mesh.h"


using namespace std;
using namespace Eigen;
using namespace glm;



// Window dimensions
const unsigned int width = 2000;
const unsigned int height = 1800;

template <typename T>
struct B_Spline
{
	T p0, p1, p2, p3;

	B_Spline() : p0(T()), p1(T()), p2(T()), p3(T())
	{
	}

	B_Spline(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3)
	{
	}

	T operator()(const float t)
	{
		const auto u = 1 - t;
		const auto tp0 = powf(u, 3) * p0;
		const auto tp1 = 3 * powf(u, 2) * t * p1;
		const auto tp2 = 3 * u * powf(t, 2) * p2;
		const auto tp3 = powf(t, 3) * p3;
		return tp0 + tp1 + tp2 + tp3;
	}
};




int main()
{
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	//	NPC npc;

	Camera camera(width, height, glm::vec3((-0.1f), 5.f, (-0.1f)));

	Bezier cubespline(3, 0.9f, .0f, 3.0f, 1000);
	vector<Mesh> SplinesMesh;
	vector<Bezier> Splines;
	//Bezier spline(1, 0.9f, .0f, 3.0f,1000);
	Light light;
	Landscape chunk("Las/Main.txt",.5f,5);
	Mesh cube(Cube);
	cube.Position.y += 10.f;
	cube.Position.z += 10.f;
	vector<Mesh> Spheres;
	ObjectBinders splinebinder;
	for(int i = 0 ; i < 40; i++)
	{
		Bezier spline(3, 0.9f, .0f, 1.0f, 100);
		Splines.emplace_back(spline);

		Mesh splinemesh(Line);
		SplinesMesh.emplace_back(splinemesh);

		Mesh sphere(Sphere);
		Spheres.emplace_back(sphere);

	}
	glm::vec3 translation = glm::vec3(10.f, 100.f, 10.f);
	for(int i = 0; i < Spheres.size();i++)
	{
		/*Splines[i].GenerateUniformKnotVector();
		Splines[i].generateBSplineVertices(1000, Splines[i].ControlPoints);*/

		SplinesMesh[i].CustomCreateSpline(Splines[i].SurfacePoints);
		Spheres[i].Position = translation;
	
		//Spheres[i].Velocity = vec3(0.1f);
		translation += vec3(10.f,0.f,10.f);
		
		
	}
	
	// Shader for light cube
	Shader lightShader("Light.vert", "Light.frag");

	Functions physics(chunk);
	NPC npc;
	Trophy trophy;
	
	vec3 pos1 = vec3(1, 0, 1);
	vec3 pos2 = vec3(5, 0, 6);
	vec3 pos3 = vec3(15, 0, 17);
	vec3 pos4 = vec3(20, 0, 5);
	auto Bez = B_Spline<vec3>(pos1,pos2,pos3,pos4);
	float t = 0.f;

	trophy.TrophyMatrix = translate(trophy.TrophyMatrix, vec3(10, 2, 10));

	
	
	Mesh Spline(Line);
	Spline.CustomCreateSpline(cubespline.SurfacePoints);
	splinebinder.Init(cubespline.SurfacePoints);
	splinebinder.Bind();
	Vertex::BindAttributes();
	
	glm::mat4 splinematrix = glm::mat4(1.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float lastFrame = 0.f;
	float time{0};
	while (!glfwWindowShouldClose(window))
	{

		//Gets the current frame
		float currentFrame = static_cast<float>(glfwGetTime());
		float Deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use

		t = fmod(t + Deltatime, 1.0f);

		shaderProgram.Activate();
		
		time += Deltatime;

		if (time > 1)
		{
			cube.BallLineStrip.emplace_back(cube.Position, glm::vec3(1.f));
			for(int i = 0; i<Spheres.size();i++)
			{
				Spheres[i].BallLineStrip.emplace_back(Spheres[i].Position, glm::vec3(1.f));
			}
			time = 0;
		}
		if (cube.BallLineStrip.size() > cubespline.FunctionGrade) {
			cubespline.OverWriteControlPoints(cube.BallLineStrip);
			cubespline.GenerateUniformKnotVector();
			cubespline.generateBSplineVertices(1000, cubespline.ControlPoints);
			splinebinder.ReBind(cubespline.SurfacePoints);
			splinebinder.Bind();
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(splinematrix));
			glLineWidth(10.f);
			glDrawArrays(GL_LINE_STRIP, 0, cubespline.SurfacePoints.size());
			
		}
		npc.NPCMatrix[3] = vec4(Bez(t), 1);
		 for(auto& triangle : chunk.indices)
		 {
			
				 glm::vec3 Barycentric = physics.Barycentric(cube.Position, triangle);

				 if (Barycentric.x >= 0 && Barycentric.y >= 0 && Barycentric.z >= 0 && (Barycentric.x + Barycentric.y + Barycentric.z <= 1)) {
					 float interpolatedy =
						 chunk.Simplifiedvertices[triangle.V0].position.y * Barycentric.x +
						 chunk.Simplifiedvertices[triangle.V2].position.y * Barycentric.y +
						 chunk.Simplifiedvertices[triangle.V1].position.y * Barycentric.z;
						 cube.Position.y = interpolatedy + (cube.Radius*2) + 4.f;
						 cube.Matrix = cube.MatrixCalc();
					 break;
				 }

		 }
		 for (int i = 0; i < Spheres.size(); i++) {
			physics.Physics(Spheres[i], Deltatime);
			cubespline.OverWriteControlPoints(Spheres[i].BallLineStrip);
			cubespline.GenerateUniformKnotVector();
			cubespline.generateBSplineVertices(1000, cubespline.ControlPoints);
			splinebinder.ReBind(cubespline.SurfacePoints);
			splinebinder.Bind();
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(splinematrix));
			glLineWidth(10.f);
			glDrawArrays(GL_LINE_STRIP, 0, cubespline.SurfacePoints.size());
			 for (int j = i + 1; j < Spheres.size(); j++) {
				 physics.Collision(Spheres[i], Spheres[j]);
			 }
			
		 }
		
	//trophy.DrawTrophy(vec3(1,1,1),shaderProgram,"model");
		 cube.Physics(chunk, Deltatime);
		 chunk.draw("model", shaderProgram);
		 npc.DrawNPC(shaderProgram, "model");
		cube.Draw( "model", shaderProgram);
		for (int i = 0; i < Spheres.size(); i++)
		{
			Spheres[i].Draw("model", shaderProgram);
			
		}

		
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light.lightPos.y, light.lightPos.z);
		
		
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 10000.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		//cube.CubeMatrix[3].y = barycentric.y;


		//Cube movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		{
			cube.Position.x += 15 * Deltatime;
			camera.Position.x += 15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		{
			cube.Position.x += -15 * Deltatime;
			camera.Position.x += -15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		{
			cube.Position.z += 15 * Deltatime;
			camera.Position.z += 15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		{
			cube.Position.z += -15 * Deltatime;
			camera.Position.z += -15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		{
			cube.Position.y += -15 * Deltatime;
			camera.Position.y += -15 * Deltatime;

		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
		{
			cube.Position.y += 15 * Deltatime;
			camera.Position.y += 15 * Deltatime;
			
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			

		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			


		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) 
		{
			Mesh ball(Sphere);
			ball.Position = camera.Position;
			Spheres.emplace_back(ball);



		}
		


		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.lightModel));
		glUniform3f(glGetUniformLocation(lightShader.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		camera.Matrix(45.f, 0.1f, 100.f, lightShader, "camMatrix");
		light.lightPos = glm::vec3(chunk.xmax, 100.f, chunk.zmax);
		light.CreateLight(vec3(1, 1, 1), vec3(1, 1, 1));
		





		//cout << "cube y: " << cube.CubeMatrix[3].y << endl;

		glGetError();

		//Camera

		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	//planevao.Delete();
	//planevbo.Delete();
	//planeebo.Delete();
	shaderProgram.Delete();
	lightShader.Delete();
	chunk.Binders.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}


