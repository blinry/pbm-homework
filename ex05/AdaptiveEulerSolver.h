#ifndef _ADAPTIVE_EULER_SOLVER_H
#define _ADAPTIVE_EULER_SOLVER_H

#include "Solver.h"

/**
 * Explicit Euler method for solving the equations of motion of a system.
 */
template <typename T>
class AdaptiveEulerSolver: public Solver<T> {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		AdaptiveEulerSolver(System<T> *system, Length threshold): Solver<T>(system), _threshold(threshold) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize) {
			// Compute the time where the system has to be after the time step.
			Time final_time = Solver<T>::_system->time + stepsize;

			size_t steps = 0;
			while (Solver<T>::_system->time < final_time) {
				// Compute the smallest step size that satisfies the specified threshold.
				Solver<T>::_system->computeAccelerations();
				Acceleration max_a;
				for (typename std::vector<typename Solver<T>::particle_type>::iterator p = Solver<T>::_system->particles.begin(); p != Solver<T>::_system->particles.end(); ++p)
					max_a = maximum(max_a, norm(p->acceleration));
				Time optimal_stepsize = 2 * quantity::sqrt(_threshold / max_a);

				// Do not advance more than necessary.
				if (Solver<T>::_system->time + optimal_stepsize > final_time)
					optimal_stepsize = final_time - Solver<T>::_system->time;

				// Perform Euler step.
				for (typename std::vector<typename Solver<T>::particle_type>::iterator p = Solver<T>::_system->particles.begin(); p != Solver<T>::_system->particles.end(); ++p) {
					p->position += optimal_stepsize * p->velocity;
					p->velocity += optimal_stepsize * p->acceleration;
				}

				// Increment the current system time.
				Solver<T>::_system->time += optimal_stepsize;
				++steps;
			}

			if (steps > 1)
				std::cout << steps << " steps in adaptive Euler solver" << std::endl;
		}
	
	private:
		Length _threshold;
};

#endif

