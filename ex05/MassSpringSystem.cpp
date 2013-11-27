#include "MassSpringSystem.h"

MassSpringSystem::MassSpringSystem(std::vector<particle_type> &particles, std::vector<Spring> &springs, ParticleDamping damping):
	System(particles), _springs(springs), _damping(damping) {}

void MassSpringSystem::computeAccelerations() {
	// Apply damping.
	for (std::vector<particle_type>::iterator p = particles.begin(); p != particles.end(); ++p)
		p->acceleration = -_damping * p->velocity / p->mass;

	// Compute forces for all springs.
	for (std::vector<Spring>::iterator spring = _springs.begin(); spring != _springs.end(); ++spring) {
		Force3D f = spring->getForce();
		spring->p1->acceleration += f / spring->p1->mass;
		spring->p2->acceleration -= f / spring->p2->mass;
	}

	// Add gravity.
	for (std::vector<particle_type>::iterator p = particles.begin(); p != particles.end(); ++p)
		p->acceleration[1] -= 9.81 * m / s / s;

	// Keep fixed particles fixed.
	for (std::vector<particle_type>::iterator p = particles.begin(); p != particles.end(); ++p)
		if (p->fixed)
			p->acceleration *= 0.0;
}

