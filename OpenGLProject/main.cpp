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
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
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


//template<typename T>
//struct Bezier
//{
//
//
//	int FunctionGrade;
//	float PointDistance;
//	float KnotMin;
//	float KnotMax;
//	int KnotSample;
//	vector<vec3> ControlPoints;
//	vector<Vertex> SurfacePoints;
//	vector<float> KnotVector_V;
//	vector<float> KnotVector_U;
//
//	Bezier(int grade, float distance, float min, float max, int size)
//		: FunctionGrade(grade), PointDistance(distance), KnotMin(min), KnotMax(max), KnotSample(size)
//	{
//	}
//	void GenerateUniformKnotVector(std::vector<float>& knotVector, int size)
//	{
//		if (size <= FunctionGrade) {
//			throw std::invalid_argument("Size must be greater than FunctionGrade.");
//		}
//
//		int n = size + FunctionGrade + 1; // Total number of knots
//		knotVector.resize(n);
//
//		// Fill in the knots
//		// First segment: KnotMin
//		for (int i = 0; i <= FunctionGrade; ++i) {
//			knotVector[i] = KnotMin;
//		}
//
//		// Middle segment: Uniformly spaced knots
//		for (int i = FunctionGrade + 1; i < n - FunctionGrade - 1; ++i) {
//			knotVector[i] = KnotMin + (KnotMax - KnotMin) * (i - FunctionGrade) / (size - 1);
//		}
//
//		// Last segment: KnotMax
//		for (int i = n - FunctionGrade - 1; i < n; ++i) {
//			knotVector[i] = KnotMax;
//		}
//	}
//	void GenerateKnotVectors(float _u, float _v)
//	{
//		GenerateUniformKnotVector(KnotVector_U, _u);
//		GenerateUniformKnotVector(KnotVector_V,_v);
//	}
//
//	//}
//	int FindKnotSpan(float t, const vector<float>& knotVector)
//	{
//		int n = knotVector.size() - 1;
//		if (t == knotVector[n]) return n - 1;
//
//		int low = FunctionGrade;
//		int high = n;
//		int mid = (low + high) / 2;
//
//		while (t < knotVector[mid] || t >= knotVector[mid + 1])
//		{
//			if (t < knotVector[mid]) high = mid;
//			else low = mid;
//			mid = (low + high) / 2;
//		}
//		return mid;
//	}
//
//	vec3 Lerp(const vec3& p1, const vec3& p2, float t)
//	{
//		return ((1.f - t) * p1 + t * p2);
//
//	}
//
//	vec3 DeBoor(int k, int i, float t, const vector<vec3>& controlPoints, const vector<float>& knotPoints)
//	{
//		if (k == 0)
//		{
//			return controlPoints[i];
//		}
//		float alpha = (t - knotPoints[i]) / (knotPoints[i + FunctionGrade] - knotPoints[i]);
//
//		vec3 point1 = DeBoor(k - 1, i - 1, t, controlPoints, knotPoints);
//		vec3 point2 = DeBoor(k - 1, i, t, controlPoints, knotPoints);
//
//		return Lerp(point1, point2, alpha);
//	}
//
//	vec3 EvaluateBiQuadratic(float _u, float _v)
//	{
//		vec3 surfacePoint(0.0f);
//
//		int numControlPointsU = ControlPoints.size() - 1; // U direction control points count
//		int numControlPointsV = ControlPoints.size() - 1; // V direction control points count (assuming square grid)
//
//		// Find knot spans for u and v
//		int knotSpanU = FindKnotSpan(_u, KnotVector_U);
//		int knotSpanV = FindKnotSpan(_v, KnotVector_V);
//
//		// Loop through control points and calculate weighted sum for surface point
//		for (int i = knotSpanU - FunctionGrade + 1; i <= knotSpanU; ++i)
//		{
//			for (int j = knotSpanV - FunctionGrade + 1; j <= knotSpanV; ++j)
//			{
//				vec3 controlPointWeight = DeBoor(FunctionGrade - 1, i, _u, ControlPoints, KnotVector_U) *
//					DeBoor(FunctionGrade - 1, j, _v, ControlPoints, KnotVector_V);
//				surfacePoint += controlPointWeight;
//			}
//		}
//		return surfacePoint;
//	}
//	void GenerateSurfacePoints(int uSamples, int vSamples)
//	{
//		for (int i = 0; i < uSamples; ++i)
//		{
//			float u = KnotMin + (KnotMax - KnotMin) * i / (uSamples - 1);
//			for (int j = 0; j < vSamples; ++j)
//			{
//				float v = KnotMin + (KnotMax - KnotMin) * j / (vSamples - 1);
//				vec3 surfacePoint = EvaluateBiQuadratic(u, v);
//				SurfacePoints.emplace_back(Vertex{ surfacePoint,vec3(1.f),vec3(1.f) });
//
//			}
//		}
//	}
//
//
//};
//
//vector<unsigned int> GenerateIndices(int uSamples, int vSamples)
//{
//	vector<unsigned int> indices;
//
//	for (int i = 0; i < uSamples - 1; ++i)
//	{
//		for (int j = 0; j < vSamples - 1; ++j)
//		{
//			int topLeft = i * vSamples + j;       // Correct row, column
//			int topRight = topLeft + 1;            // Correct row, column + 1
//			int bottomLeft = (i + 1) * vSamples + j; // Next row, same column
//			int bottomRight = bottomLeft + 1;      // Next row, same column + 1
//
//			// Ensure these indices are within bounds
//			indices.push_back(topLeft);
//			indices.push_back(bottomLeft);
//			indices.push_back(bottomRight);
//			indices.push_back(topLeft);
//			indices.push_back(bottomRight);
//			indices.push_back(topRight);
//		}
//	}
//	return indices;
//}






float curveplane(float x, float y)
{

	return cos(x) + sin(y) - cos(y);
}



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

	//Bezier<float> bezier(3, 0.5f, -2.0f, 3.0f,7);
	//bezier.ControlPoints = {
	//	 glm::vec3(0.0f, 0.0f, 0.0f),  // Point 0 (Bottom Left)
	//glm::vec3(1.0f, 0.0f, 0.0f),  // Point 1 (Bottom Right)
	//glm::vec3(2.0f, 0.0f, 0.0f),  // Point 2 (Bottom Right)
	//glm::vec3(3.0f, 0.0f, 0.0f),  // Point 3 (Bottom Right)

	//glm::vec3(0.0f, 1.0f, 0.0f),  // Point 4 (Middle Left)
	//glm::vec3(1.0f, 1.0f, 0.0f),  // Point 5 (Middle Right)
	//glm::vec3(2.0f, 1.0f, 0.0f),  // Point 6 (Middle Right)
	//glm::vec3(3.0f, 1.0f, 0.0f),  // Point 7 (Middle Right)

	//glm::vec3(0.0f, 2.0f, 0.0f),  // Point 8 (Top Left)
	//glm::vec3(1.0f, 2.0f, 0.0f),  // Point 9 (Top Right)
	//glm::vec3(2.0f, 2.0f, 0.0f),  // Point 10 (Top Right)
	//glm::vec3(3.0f, 2.0f, 0.0f)   // Point 11 (Top Right)

	//};

	//int u = 4;
	//int v = 6;
	//bezier.GenerateKnotVectors(u,v);

	//// Generate surface points for 20x20 samples
	//bezier.GenerateSurfacePoints(u,v);
	//vector<unsigned int> Indices = GenerateIndices(16,18);

	//mat4 Doormatrix = mat4(1.0f);
	//Doormatrix = translate(Doormatrix, vec3(-1.f));


	Light light;

	
	//

	Landscape chunk("Las/Main.txt",0.1f,2);
	//chunk.Matrix = glm::scale(chunk.Matrix, vec3(0.01f));
	//chunk.Matrix = glm::translate(chunk.Matrix, vec3(0.f,10.f,0.f));
	Mesh cube(Cube);
	vector<Mesh> Spheres;
	for(int i = 0 ; i < 20; i++)
	{

		Spheres.emplace_back(Mesh(Sphere));

	}
	for (int i = 0; i < Spheres.size();i++)
	{

		Spheres[i].Matrix = glm::translate(Spheres[i].Matrix, vec3(rand()%100));
	}
	// Shader for light cube
	Shader lightShader("Light.vert", "Light.frag");


	NPC npc;
	Trophy trophy;
	
	vec3 pos1 = vec3(1, 0, 1);
	vec3 pos2 = vec3(5, 0, 6);
	vec3 pos3 = vec3(15, 0, 17);
	vec3 pos4 = vec3(20, 0, 5);
	auto Bez = B_Spline<vec3>(pos1,pos2,pos3,pos4);
	float t = 0.f;

	trophy.TrophyMatrix = translate(trophy.TrophyMatrix, vec3(10, 2, 10));
	cube.Matrix = translate(cube.Matrix, vec3(0, 0, 7));


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float lastFrame = 0.f;

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
		chunk.draw("model", shaderProgram);
		npc.DrawNPC(shaderProgram, "model");

		npc.NPCMatrix[3] = vec4(Bez(t), 1);
		
		for(auto& triangles : chunk.indices)
		{
			//cout << "trying to calculate barycentric" << endl;
			if (chunk.IsInsideTriangle(triangles, cube.Matrix[3])) {
				glm::vec3 position = cube.Matrix[3];
				vec3 interpolatedy = chunk.Barycentric(position, triangles);
				cube.Matrix[3].y =  interpolatedy.y;
				break;
				//cout << "interpolated y value to be: " << interpolatedy.y << endl;
			}
			for(int i = 0; i < Spheres.size();i++)
			{

				//cout << "trying to calculate barycentric" << endl;
				if (chunk.IsInsideTriangle(triangles, Spheres[i].Matrix[3])) {
					glm::vec3 position = Spheres[i].Matrix[3];
					vec3 interpolatedy = chunk.Barycentric(position, triangles);
					Spheres[i].Matrix[3].y = interpolatedy.y;
					break;
					//cout << "interpolated y value to be: " << interpolatedy.y << endl;
				}
			}
		}
		trophy.DrawTrophy(vec3(1,1,1),shaderProgram,"model");
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light.lightPos.y, light.lightPos.z);
		
		cube.Draw( "model", shaderProgram);

		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 1000.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		//cube.CubeMatrix[3].y = barycentric.y;


		//Cube movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		{
			cube.Matrix[3].x += 5 * Deltatime;
			camera.Position.x += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		{
			cube.Matrix[3].x += -5 * Deltatime;
			camera.Position.x += -5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		{
			cube.Matrix[3].z += 5 * Deltatime;
			camera.Position.z += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		{
			cube.Matrix[3].z += -5 * Deltatime;
			camera.Position.z += -5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //forward
		{
			cube.Matrix[3].y += -5 * Deltatime;
			camera.Position.y += -5 * Deltatime;

		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //forward
		{
			cube.Matrix[3].y += 5 * Deltatime;
			camera.Position.y += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) //forward
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//seeWireFrame = true;
			

		}
		


		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.lightModel));
		glUniform3f(glGetUniformLocation(lightShader.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		camera.Matrix(45.f, 0.1f, 100.f, lightShader, "camMatrix");
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


