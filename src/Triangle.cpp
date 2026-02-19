#include "Triangle.h"

Triangle::Triangle(Particle& p1, Particle& p2, Particle& p3, float fluidDensity, float dragCoefficient) {
	vertices.push_back(&p1);
	vertices.push_back(&p2);
	vertices.push_back(&p3);

	// Compute the normal vector of the triangle
	glm::vec3 edge1 = p2.position - p1.position;
	glm::vec3 edge2 = p3.position - p1.position;
	normal = glm::normalize(glm::cross(edge1, edge2));

	this->fluidDensity = fluidDensity;
	this->dragCoefficient = dragCoefficient;

	this->area = 0.0f;
}

Triangle::~Triangle() {
 }

void Triangle::Update(float deltaTime, glm::vec3 windVelocity) {
	// Compute the normal vector of the triangle
	glm::vec3 edge1 = vertices[1]->position - vertices[0]->position;
	glm::vec3 edge2 = vertices[2]->position - vertices[0]->position;
	normal = glm::normalize(glm::cross(edge1, edge2));

	// Compute the area of the triangle
	area = 0.5f * glm::length(glm::cross(edge1, edge2));
	float normalArea = area * glm::dot(normal, glm::normalize(windVelocity));

	// Compute the drag force on the triangle
	glm::vec3 relativeVelocity = windVelocity - (vertices[0]->velocity + vertices[1]->velocity + vertices[2]->velocity) / 3.0f;
	glm::vec3 dragForce = -0.5f * fluidDensity * glm::length(relativeVelocity) * glm::length(relativeVelocity) * dragCoefficient * normalArea * normal;
	vertices[0]->ApplyForce(-dragForce / 3.0f);
	vertices[1]->ApplyForce(-dragForce / 3.0f);
	vertices[2]->ApplyForce(-dragForce / 3.0f);
}