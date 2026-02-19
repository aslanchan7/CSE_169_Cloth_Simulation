#pragma once
#include "Particle.h"
#include "SpringDamper.h"
#include "Triangle.h"
#include "Mesh.h"
#include <vector>

class Cloth {
public:
	Cloth(float springConstant, float dampingConstant, float fluidDensity, float dragCoefficient, 
			float restitutionCoefficient, float frictionCoefficient, 
			glm::vec3 windDir, float windSpeed
	);
	~Cloth();

	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Update(float deltaTime);

	void MoveFixedParticles(float d);

	// public variables for ImGui
	float windSpeed;

private:
	std::vector<Particle*> particles;
	std::vector<SpringDamper*> springDampers;
	std::vector<Triangle*> triangles;

	glm::vec3 windDirection;

	float restitutionCoefficient = 0.05f;
	float frictionCoefficient = 0.6f;

	// Data for mesh
	Mesh* mesh;
	std::vector<glm::ivec3> triangleIndices;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
};