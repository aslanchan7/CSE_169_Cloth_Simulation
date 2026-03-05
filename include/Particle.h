#pragma once
#include "core.h"
#include "Sphere.h"

class Particle {
public:
	Particle(float mass, glm::vec3 initPos, glm::vec3 initVelocity, bool fixed);
	~Particle();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update(float deltaTime);
	void ApplyForce(const glm::vec3& force);
	void ApplyImpulse(const glm::vec3& impulse);

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;

	bool fixed;

	Sphere* sphere;

	// Adjustable Settings
	float radius;
	float dragCoefficient;
};