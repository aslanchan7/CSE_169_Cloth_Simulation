#pragma once
#include "core.h"

class Particle {
public:
	Particle();
	~Particle();

	void Update(float deltaTime);
	void ApplyForce(const glm::vec3& force);
	void ApplyImpulse(const glm::vec3& impulse);

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;

	bool fixed;
};