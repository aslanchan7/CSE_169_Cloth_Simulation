#include "SpringDamper.h"

SpringDamper::SpringDamper() {
	springConstant = 0.0f;
	dampingConstant = 0.0f;
	restLength = 0.0f;
	p1 = nullptr;
	p2 = nullptr;
}

SpringDamper::~SpringDamper() {
	//if (p1 != nullptr) {
	//	delete p1;
	//}

	//if (p2 != nullptr) {
	//	delete p2;
	//}
}

void SpringDamper::ComputeForces() {
	if (p1 == nullptr || p2 == nullptr) return;

	// Vector from p1 to p2
	glm::vec3 dir = p2->position - p1->position;
	float currentLength = glm::length(dir);
	glm::vec3 normalizedDir = glm::normalize(dir);
	
	// Spring force
	float springForceMagnitude = springConstant * (currentLength - restLength);
	glm::vec3 springForce = springForceMagnitude * normalizedDir;

	// Damping force
	float closingVelocity = glm::dot((p2->velocity - p1->velocity), normalizedDir);
	float dampingForceMagnitude = dampingConstant * closingVelocity;
	glm::vec3 dampingForce = dampingForceMagnitude * normalizedDir;

	// Total force
	glm::vec3 totalForce = springForce + dampingForce;

	// Apply forces to particles
	p1->ApplyForce(totalForce);
	p2->ApplyForce(-totalForce);
}