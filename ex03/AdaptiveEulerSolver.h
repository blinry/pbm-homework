#ifndef _ADAPTIVE_EULER_SOLVER_H
#define _ADAPTIVE_EULER_SOLVER_H

#include "Solver.h"

/**
 * Explicit Euler method for solving the equations of motion of a system.
 */
class AdaptiveEulerSolver: public Solver {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		AdaptiveEulerSolver(System *system, Length threshold): Solver(system), _threshold(threshold) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize);
	
	private:
		Length _threshold;
};

#endif

