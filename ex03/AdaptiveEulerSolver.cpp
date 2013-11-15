#include "AdaptiveEulerSolver.h"

void AdaptiveEulerSolver::step(const Time stepsize) {
	// Compute the time where the system has to be after the time step.
	Time final_time = _system->time + stepsize;

	size_t steps = 0;
	while (_system->time < final_time) {
		// Compute the smallest step size that satisfies the specified threshold.
		_system->computeAccelerations();
		Acceleration max_a;
		for (std::vector<Particle>::iterator p = _system->particles.begin(); p != _system->particles.end(); ++p)
			max_a = maximum(max_a, norm(p->acceleration));
		Time optimal_stepsize = 2 * quantity::sqrt(_threshold / max_a);

		// Do not advance more than necessary.
		if (_system->time + optimal_stepsize > final_time)
			optimal_stepsize = final_time - _system->time;

		// Perform Euler step.
		for (std::vector<Particle>::iterator p = _system->particles.begin(); p != _system->particles.end(); ++p) {
			p->position += optimal_stepsize * p->velocity;
			p->velocity += optimal_stepsize * p->acceleration;
		}

		// Increment the current system time.
		_system->time += optimal_stepsize;
		++steps;
	}

	if (steps > 1)
		std::cout << steps << " steps in adaptive Euler solver" << std::endl;
}

