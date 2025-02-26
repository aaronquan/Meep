#include "geometry.hpp"



QuadraticSolution solveQuadratic(float a, float b, float c) {
	if (a == 0) {
		if (b == 0) {
			return QuadraticSolution(QuadraticSolutionType::NoRoot);
		}
		return QuadraticSolution(QuadraticSolutionType::OneRoot, -c / b);
	}
	float discriminant = (b * b) - (4 * a * c);
	if (discriminant > 0) {
		float sqroot_discriminant = std::sqrt(discriminant);
		float divisor = 2 * a;
		float x1 = (sqroot_discriminant - b) / divisor;
		float x2 = -(b + sqroot_discriminant) / divisor;
		return QuadraticSolution(QuadraticSolutionType::TwoRoots, x1, x2);
	}
	else if (discriminant < 0) {
		return QuadraticSolution(QuadraticSolutionType::NoRoot);
	}
	if (a == 0) {

		return QuadraticSolution(QuadraticSolutionType::OneRoot, -c / b);
	}
	float sqroot_discriminant = std::sqrt(discriminant);
	float divisor = 2 * a;
	float single_solution = (sqroot_discriminant - b) / divisor;
	return QuadraticSolution(QuadraticSolutionType::OneRoot, single_solution);
}

std::string quadraticSolutionToString(QuadraticSolution qs) {
	switch (qs.type) {
	case QuadraticSolutionType::NoRoot:
		return std::string("No root");
	case QuadraticSolutionType::OneRoot:
		return std::format("One root: {}", qs.x1);
	case QuadraticSolutionType::TwoRoots:
		return std::format("Two roots: {} {}", qs.x1, qs.x2);
	}

}

QuadraticCoefficients getLinearPolynomialSquareCoefficients(float a, float b) {
	return QuadraticCoefficients(a * a, 2 * a * b, b * b);
}