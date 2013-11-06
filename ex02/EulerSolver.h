#ifndef _EULER_SOLVER_H
#define _EULER_SOLVER_H

#include "Solver.h"

/**
 * Explicit Euler method for solving the equations of motion of a system.
 */
class EulerSolver: public Solver {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		EulerSolver(System *system): Solver(system) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize);
};

#endif

