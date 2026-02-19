#pragma once
#include "Particle.h"

class SpringDamper {
public:
	SpringDamper();
	~SpringDamper();

	void ComputeForces();
	
	Particle* p1, *p2;
	
	float springConstant;
	float dampingConstant;
	float restLength;
};