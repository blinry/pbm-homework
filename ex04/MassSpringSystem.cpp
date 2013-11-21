#include "MassSpringSystem.h"

MassSpringSystem::MassSpringSystem(std::vector<Particle> &particles, std::vector<Spring> &springs, std::vector<Obstacle*> &obstacles, ParticleDamping damping):
	System(particles), _springs(springs), _obstacles(obstacles), _damping(damping) {}

void MassSpringSystem::computeAccelerations() {
	// Apply damping.
	for (std::vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
		p->acceleration = -_damping * p->velocity / p->mass;

	// Compute forces for all springs.
	for (std::vector<Spring>::iterator spring = _springs.begin(); spring != _springs.end(); ++spring) {
		Force3D f = spring->getForce();
		spring->p1->acceleration += f / spring->p1->mass;
		spring->p2->acceleration -= f / spring->p2->mass;
	}

	// Add gravity.
	for (std::vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
		p->acceleration[1] -= 9.81 * m / s / s;

	// Detect and handle collisions with a plane.
	for (std::vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p) {
		for (std::vector<Obstacle*>::iterator q = _obstacles.begin(); q != _obstacles.end(); ++q) {
			p->acceleration += (*q)->computeFrictionForce(*p) / p->mass;
			p->acceleration += (*q)->computeReflectionForce(*p) / p->mass;
		}
	}
		
	// Keep fixed particles fixed.
	for (std::vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p)
		if (p->fixed)
		{
			p->acceleration *= 0.0;
			p->velocity *= 0.0;
		}
}

