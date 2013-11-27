#ifndef _EULER_SOLVER_H
#define _EULER_SOLVER_H

#include "Solver.h"

/**
 * Explicit Euler method for solving the equations of motion of a system.
 */
template <typename T>
class EulerSolver: public Solver<T> {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		EulerSolver(System<T> *system): Solver<T>(system) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize) {
			// Compute the accelerations exerted on the particles.
			Solver<T>::_system->computeAccelerations();

			// Increment the position and velocity of each particle by the current velocity
			// and acceleration, respectively.
			for (typename std::vector<typename Solver<T>::particle_type>::iterator p = Solver<T>::_system->particles.begin(); p != Solver<T>::_system->particles.end(); ++p) {
				p->position += stepsize * p->velocity;
				p->velocity += stepsize * p->acceleration;
			}

			// Increment the current system time.
			Solver<T>::_system->time += stepsize;
		}
};

#endif

