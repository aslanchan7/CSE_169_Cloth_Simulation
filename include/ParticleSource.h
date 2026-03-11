#pragma once
#include "core.h"
#include <vector>
#include "Particle.h"

class ParticleSource {
public:
	ParticleSource(glm::vec3 position, glm::vec3 velocity, float spawnRate);
	~ParticleSource();
	
	void Draw(const glm::mat4& viewProjMtx, GLuint shader) {
		for (Particle* p : particles) {
			p->Draw(viewProjMtx, shader);
		}
	}
	void Update(float deltaTime);
	void SpawnParticle();
	void DeleteParticle(Particle* particle) {
		auto it = std::find(particles.begin(), particles.end(), particle);
		if (it != particles.end()) {
			delete* it;
			particles.erase(it);
		}
	}

	//glm::vec3 spawnPos;
	//glm::vec3 spawnVelocity;
	//float spawnRate; // Particles per second

	std::vector<Particle*> particles;
};