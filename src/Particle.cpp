#include "Particle.h"
#include "Window.h"
#include <iostream>

Particle::Particle(float mass, glm::vec3 initPos, glm::vec3 initVelocity, bool fixed) {
	this->mass = mass;
	position = initPos;
	velocity = initVelocity;
	force = glm::vec3(0);
	this->fixed = fixed;

	dragCoefficient = Window::dragCoefficient;
	radius = Window::particleRadius;
	
	sphere = new Sphere(position, radius);
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

	// GRAVITY
	ApplyForce(glm::vec3(0.0f, -9.81f * Window::gravityScale * mass, 0.0f));

	// AERODYNAMIC DRAG
	if (abs(Window::windSpeed) > 0.001f) {
		glm::vec3 relVelocity = (Window::windDir * Window::windSpeed) - this->velocity;
		float relSpeed = glm::length(relVelocity);
		float area = 3.14159 * pow(radius, 2);
		glm::vec3 dragDir = -relVelocity / relSpeed;
		glm::vec3 aeroDragForce = 0.5f * Window::airDensity * float(pow(relSpeed, 2)) *
									dragCoefficient * area * dragDir;
		
		ApplyForce(aeroDragForce);
	}

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