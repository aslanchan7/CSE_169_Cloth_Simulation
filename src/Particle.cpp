#include "Particle.h"

Particle::Particle() {
	position = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	force = glm::vec3(0.0f);
	mass = 0.0f;
	fixed = false;
}

Particle::~Particle() {
}

void Particle::ApplyForce(const glm::vec3& force) {
	this->force += force;
}

void Particle::Update(float deltaTime) {
	if (fixed) return;

	// Newton's Second Law
	glm::vec3 acceleration = force / mass;
	
	// Forward Euler Integration
	velocity += acceleration * deltaTime;
	
	// Backward Euler Integration
	position += velocity * deltaTime;
	
	// Zero out the force for next frame
	force = glm::vec3(0.0f); 
}