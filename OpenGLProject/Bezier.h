#pragma once
#include <iostream>
#include <vector>

#include "Vertex.h"

class Bezier
{
public:

	int FunctionGrade{0};
	float PointDistance{0};
	float KnotMin{0};
	float KnotMax{0};
	int KnotSample{0};
	std::vector<glm::vec3> ControlPoints{0};
	std::vector<Vertex> SurfacePoints{0};
	std::vector<float> KnotVector_V{0};
	std::vector<float> KnotVector_U{0};
	float step{0};

	Bezier(int grade, float distance, float min, float max, int size)
		: FunctionGrade(grade), PointDistance(distance), KnotMin(min), KnotMax(max), KnotSample(size)
	{
	}
	void GenerateUniformKnotVector()
	{
		KnotVector_U.clear();
		KnotVector_V.clear();
		int size =  ControlPoints.size() + FunctionGrade +1;
		int availablepoints = size - (FunctionGrade+1 * 2);
		step = (KnotMax - KnotMin) / availablepoints;
	
		int distance = size - ((FunctionGrade + 1) * 2);

		for (int i = 0; i < FunctionGrade; i++) {
			KnotVector_U.emplace_back(KnotMin);
			KnotVector_V.emplace_back(KnotMin);
		}
		for (int i = 1; i < availablepoints; i++)
		{
			KnotVector_U.emplace_back(KnotMin + (step * i));
			KnotVector_V.emplace_back(KnotMin + (step * i));
		}
		for (int i = 0; i < FunctionGrade; i++)
		{
			KnotVector_U.emplace_back(KnotMax);
			KnotVector_V.emplace_back(KnotMax);

		}

	}

	glm::vec3 Lerp(const glm::vec3& p1, const glm::vec3& p2, float t)
	{
		return ((1.f - t) * p1 + t * p2);

	}

	glm::vec3 DeBoor(int k, int i, float t, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knotPoints) {
		if (k == 0) {
			return controlPoints[i];
		}

		float alpha = 0.0f;
		float denominator = knotPoints[i + k] - knotPoints[i];
		if (denominator != 0.0f) { // Avoid division by zero
			alpha = (t - knotPoints[i]) / denominator;
		}

		glm::vec3 point1 = DeBoor(k - 1, i - 1, t, controlPoints, knotPoints);
		glm::vec3 point2 = DeBoor(k - 1, i, t, controlPoints, knotPoints);

		return Lerp(point1, point2, alpha); // Linear interpolation
	}


	int findKnotSpan(float t, const std::vector<float>& knotPoints) {
		int n = knotPoints.size() - FunctionGrade - 1;
		if (t >= knotPoints[n]) return n - 1;

		for (int i = FunctionGrade; i < n; ++i) {
			if (t >= knotPoints[i] && t < knotPoints[i + 1]) {
				return i;
			}
		}

		return -1; // Error case
	}

	glm::vec3 evaluateBSpline(float t, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knotPoints) {
		int span = findKnotSpan(t, knotPoints);
		if (span == -1) return glm::vec3(0.0f); // Handle invalid cases
		return DeBoor(FunctionGrade, span, t, controlPoints, knotPoints);
	}

	void generateBSplineVertices(int samples, std::vector<glm::vec3>& controlpoints) {

		if (controlpoints.empty()) {
			std::cerr << "Error: ControlPoints is empty!" << std::endl;
			return;
		}
		// Check if KnotVector_U is sufficiently large
		if (KnotVector_U.size() <= FunctionGrade) {
			std::cout << "Error: KnotVector_U does not have enough elements!" << std::endl;
			return; // Early return if knot vector is not populated correctly
		}

		// Check if controlpoints are sufficiently large
		if (controlpoints.size() < FunctionGrade) {
			std::cout << "Error: Not enough control points!" << std::endl;
			return; // Early return if there are not enough control points
		}
		SurfacePoints.clear();
		float tStart = KnotVector_U[FunctionGrade + 1];
		float tEnd = KnotVector_U[KnotVector_U.size() - FunctionGrade - 1];
		float step = (tEnd - tStart) / samples;

		for (float t = tStart; t < tEnd; t += step) {
			glm::vec3 point = evaluateBSpline(t, controlpoints, KnotVector_U);
			SurfacePoints.emplace_back(Vertex{ point, glm::vec3(1.f, 0.f, 0.f) }); // Red color
		}
	}
	void OverWriteControlPoints(std::vector<Vertex>& newpoints)
	{
		ControlPoints.clear();
		for (int i = 0; i < newpoints.size(); i++) {
			ControlPoints.emplace_back(newpoints[i].position);
		}
	}
};

