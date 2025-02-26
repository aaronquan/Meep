#pragma once
#include <cmath>
#include <format>
#include <string>

enum class QuadraticSolutionType {
	NoRoot, OneRoot, TwoRoots
};

struct QuadraticSolution {
	QuadraticSolutionType type;
	float x1;
	float x2;
};

QuadraticSolution solveQuadratic(float a, float b, float c);

std::string quadraticSolutionToString(QuadraticSolution qs);

struct QuadraticCoefficients {
	float a;
	float b;
	float c;
};

// (ax + b)^2
QuadraticCoefficients getLinearPolynomialSquareCoefficients(float a, float b);