#pragma once
#include "core.h"
#include "Particle.h"
#include <vector>

class Triangle {
public:
	Triangle(Particle& p1, Particle& p2, Particle& p3, float fluidDensity, float dragCoefficient);
	~Triangle();

	void Update(float deltaTime, glm::vec3 windVelocity);

	std::vector<Particle*> vertices;
	glm::vec3 normal;

	float fluidDensity;
	float dragCoefficient;
	float area;
};