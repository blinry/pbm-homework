#ifndef _SOLVER_H
#define _SOLVER_H

#include "System.h"

/**
 * Abstract base class for methods for solving the equations of motion of a
 * system.
 */
template <typename T>
class Solver {
	public:
		typedef T particle_type;

		/**
		 * Construct a new solver for a given system.
		 */
		Solver(System<T> *system): _system(system) {}

		virtual ~Solver() {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		virtual void step(const Time stepsize) = 0;

	protected:
		System<T> *_system; //< The system on which the solver operates.
};

#endif

