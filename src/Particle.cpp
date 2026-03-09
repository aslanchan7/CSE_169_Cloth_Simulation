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
	restitutionCoefficient = 0.3f;
	frictionCoefficient = 0.6f;
	
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
		glm::vec3 windVelocity = Window::windDir * Window::windSpeed;
		
		// Velocity of particle along wind direction
		glm::vec3 particleVelAlongWind = glm::dot(this->velocity, Window::windDir) * Window::windDir;

		glm::vec3 relVelocity = particleVelAlongWind - windVelocity;
		float relSpeed = glm::length(relVelocity);

		if (relSpeed < 0.001f) {
			return; // No significant relative velocity, skip drag calculation
		}

		float area = 3.14159f * pow(radius, 2);
		glm::vec3 dragDir = relVelocity / relSpeed;
		glm::vec3 aeroDragForce = 0.5f * Window::airDensity * float(pow(relSpeed, 2)) *
									dragCoefficient * area * dragDir;
		std::cout << "Drag Dir: " << dragDir.x << ", " << dragDir.y << ", " << dragDir.z << std::endl;
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

	// Collision Detection
	if (position.y <= -2.0f) {
		// Calculate impulse and apply it to the particle
		glm::vec3 impulseVec = -(1.0f + restitutionCoefficient) * mass * velocity.y * glm::vec3(0.0f, 1.0f, 0.0f);
		ApplyImpulse(impulseVec);

		// Set position to be on the plane
		position.y = -2.0f;

		// Apply friction
		glm::vec3 frictionVec = -frictionCoefficient * velocity * glm::vec3(1.0f, 0.0f, 1.0f);
		ApplyForce(frictionVec);
	}

	// Update render object (mesh)
	sphere->Update(position);
}

void Particle::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	sphere->Update(position);
	sphere->Draw(viewProjMtx, shader);
}