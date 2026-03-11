#include "ParticleSource.h"
#include "Window.h"

ParticleSource::ParticleSource(glm::vec3 position, glm::vec3 velocity, float spawnRate) {
	//this->spawnPos = position;
	//this->spawnVelocity = velocity;
	//this->spawnRate = spawnRate;
}

ParticleSource::~ParticleSource() {
	for (Particle* p : particles) {
		delete p;
	}
}

void ParticleSource::Update(float deltaTime) {
	// Spawn new particles based on the spawn rate
	static float timeSinceLastSpawn = 0.0f;
	timeSinceLastSpawn += deltaTime;
	while (timeSinceLastSpawn >= 1.0f / Window::spawnRate) {
		SpawnParticle();
		timeSinceLastSpawn -= 1.0f / Window::spawnRate;
	}

	// Update existing particles
	for (Particle* p : particles) {
		p->Update(deltaTime);
		if (p->lifespan < 0.0f) {
			DeleteParticle(p);
		}
	}
}

void ParticleSource::SpawnParticle() {
	// Add some random variance to the spawn position and velocity
	glm::vec3 randomPosVariance = glm::vec3(
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::spawnPosVariance.x,
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::spawnPosVariance.y,
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::spawnPosVariance.z
	);

	glm::vec3 randomVelVariance = glm::vec3(
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::velocityVariance.x,
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::velocityVariance.y,
		((rand() / (float)RAND_MAX) - 0.5f) * 2.0f * Window::velocityVariance.z
	);

	Particle* newParticle = new Particle(0.1f, Window::spawnPos + randomPosVariance, 
								Window::spawnVelocity + randomVelVariance, false);
	particles.push_back(newParticle);
}

