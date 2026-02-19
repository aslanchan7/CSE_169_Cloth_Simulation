#include "Cloth.h"

Cloth::Cloth(float springConstant, float dampingConstant, float fluidDensity, float dragCoefficient, glm::vec3 windDir, float windSpeed) {
	// Create particles
	int numParticlesX = 10;
	int numParticlesY = 10;
	float spacing = 0.1f;
	for (int y = 0; y < numParticlesY; y++) {
		for (int x = 0; x < numParticlesX; x++) {
			Particle* p = new Particle();

			float randomNum = rand() % 3;
			if (randomNum <= 1.0) p->position = glm::vec3(x * spacing, -y * (spacing / 2.0f), 0);
			else if (randomNum <= 2.0) p->position = glm::vec3(x * spacing, -y * (spacing / 2.0f), 0.05f);
			else p->position = glm::vec3(x * spacing, -y * (spacing / 2.0f), -0.05f);
			//p->position = glm::vec3(x * spacing, -y * (spacing/2.0f), 0);
			
			if (y == 0) {
				p->fixed = true; // Fix the top row of particles
				p->position.z = 0.0f;
			}

			p->mass = 0.015f; // ~15g per particle
			particles.push_back(p);
			positions.push_back(p->position);
		}
	}

	// Create spring-dampers
	for (int y = 0; y < numParticlesY; y++) {
		for (int x = 0; x < numParticlesX; x++) {
			int index = y * numParticlesX + x;

			// Create spring-damper to the right
			if (x < numParticlesX - 1) {
				SpringDamper* sd = new SpringDamper();
				sd->p1 = particles[index];
				sd->p2 = particles[index + 1];
				sd->springConstant = springConstant;	
				sd->dampingConstant = dampingConstant;	
				sd->restLength = spacing;
				springDampers.push_back(sd);
			}

			// Create spring-damper to the bottom
			if (y < numParticlesY - 1) {
				SpringDamper* sd = new SpringDamper();
				sd->p1 = particles[index];
				sd->p2 = particles[index + numParticlesX];
				sd->springConstant = springConstant;		
				sd->dampingConstant = dampingConstant;		
				sd->restLength = spacing;
				springDampers.push_back(sd);
			}

			// Create spring-damper to the bottom-right
			if (x < numParticlesX - 1 && y < numParticlesY - 1) {
				SpringDamper* sd = new SpringDamper();
				sd->p1 = particles[index];
				sd->p2 = particles[index + numParticlesX + 1];
				sd->springConstant = springConstant;
				sd->dampingConstant = dampingConstant;
				sd->restLength = spacing * glm::sqrt(2.0f);
				springDampers.push_back(sd);
			}

			// Create spring-damper to the bottom-left
			if (x > 0 && y < numParticlesY - 1) {
				SpringDamper* sd = new SpringDamper();
				sd->p1 = particles[index];
				sd->p2 = particles[index + numParticlesX - 1];
				sd->springConstant = springConstant;		
				sd->dampingConstant = dampingConstant;
				sd->restLength = spacing * glm::sqrt(2.0f);
				springDampers.push_back(sd);
			}
		}
	}

	// Create triangles
	for (int y = 0; y < numParticlesY; y++) {
		for (int x = 0; x < numParticlesX; x++) {
			int index = y * numParticlesX + x;
			// Create triangle to the right and bottom
			if (x < numParticlesX - 1 && y < numParticlesY - 1) {
				Triangle* t1 = new Triangle(
					*particles[index],
					*particles[index + 1],
					*particles[index + numParticlesX],
					fluidDensity,
					dragCoefficient
				);
				triangles.push_back(t1);
				// Mesh Data
				triangleIndices.push_back(glm::ivec3(index, index + 1, index + numParticlesX));

				Triangle* t2 = new Triangle(
					*particles[index + 1], 
					*particles[index + numParticlesX + 1], 
					*particles[index + numParticlesX],
					fluidDensity,
					dragCoefficient
				);
				triangles.push_back(t2);
				// Mesh Data
				triangleIndices.push_back(glm::ivec3(index + 1, index + numParticlesX + 1, index + numParticlesX));
			}
		}
	}

	// Initialize normals
	normals.resize(particles.size());
	for (int i = 0; i < triangleIndices.size(); i++)
	{
		normals[triangleIndices[i].x] = triangles[i]->normal;
		normals[triangleIndices[i].y] = triangles[i]->normal;
		normals[triangleIndices[i].z] = triangles[i]->normal;
	}

	// Create mesh
	mesh = new Mesh(positions, normals, triangleIndices);

	// Initialize wind
	this->windDirection = glm::normalize(windDir);
	this->windSpeed = windSpeed;
}

Cloth::~Cloth() {
	for (Particle* p : particles) {
		if (p != nullptr) delete p;
	}

	for (SpringDamper* sd : springDampers) {
		if (sd != nullptr) delete sd;
	}
	for (Triangle* t : triangles) {
		if (t != nullptr) delete t;
	}
}

void Cloth::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	// TODO: Draw the cloth
	mesh->Draw(viewProjMtx, shader);
}

void Cloth::Update(float deltaTime) {
	for (SpringDamper* sd : springDampers) {
		sd->ComputeForces();
	}

	for (Particle* p : particles) {
		p->ApplyForce(glm::vec3(0.0f, -9.81f * p->mass, 0.0f)); // Gravity
		p->Update(deltaTime);
	}

	for (Triangle* t : triangles) {
		t->Update(deltaTime, windSpeed * windDirection);
	}

	// Update mesh vertices
	for (int i = 0; i < particles.size(); i++) {
		positions[i] = particles[i]->position;
	}

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		normals[triangleIndices[i].x] = triangles[i]->normal;
		normals[triangleIndices[i].y] = triangles[i]->normal;
		normals[triangleIndices[i].z] = triangles[i]->normal;
	}

	mesh->UpdateVertices(positions, normals);
}