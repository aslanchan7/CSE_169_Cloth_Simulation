#include "Particle.h"
#include <iostream>

Particle::Particle(float mass, glm::vec3 initPos, glm::vec3 initVelocity, bool fixed) {
	this->mass = mass;
	position = initPos;
	velocity = initVelocity;
	force = glm::vec3(0);
	this->fixed = fixed;
	sphere = new Sphere(position);
}

Particle::~Particle() {
	delete sphere;
}

void Particle::ApplyForce(const glm::vec3& force) {
	this->force += force;
}

void Particle::ApplyImpulse(const glm::vec3& impulse) {
	if (fixed) return;
	velocity += impulse / mass;
}

void Particle::Update(float deltaTime) {
	if (fixed) return;

	// Every update, apply gravity
	ApplyForce(glm::vec3(0.0f, -9.81f * mass, 0.0f));

	// Newton's Second Law
	glm::vec3 acceleration = force / mass;
	
	// Forward Euler Integration
	velocity += acceleration * deltaTime;
	
	// Backward Euler Integration
	position += velocity * deltaTime;
	
	// Zero out the force for next frame
	force = glm::vec3(0.0f); 

	// Update render object (mesh)
	sphere->Update(position);
}

void Particle::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	sphere->Update(position);
	sphere->Draw(viewProjMtx, shader);
}