#include "EulerSolver.h"

void EulerSolver::step(const Time stepsize) {
	// Compute the accelerations exerted on the particles.
	_system->computeAccelerations();

	// Increment the position and velocity of each particle by the current velocity
	// and acceleration, respectively.
	for (std::vector<Particle>::iterator p = _system->particles.begin(); p != _system->particles.end(); ++p) {
		p->position += stepsize * p->velocity;
		p->velocity += stepsize * p->acceleration;
	}

	// Increment the current system time.
	_system->time += stepsize;
}

