#ifndef _RUNGE_KUTTA_SOLVER_H
#define _RUNGE_KUTTA_SOLVER_H

#include "Solver.h"
using namespace std;

/**
 * Runge-Kutta method for solving the equations of motion of a system.
 */
class RungeKuttaSolver: public Solver {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		RungeKuttaSolver(System *system): Solver(system) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize);
        void compAcc(vector<Length3D> r, vector<Acceleration3D>* a);
        void eulerStep(vector<Length3D> r, vector<Velocity3D> v, vector<Acceleration3D> a, vector<Length3D>* r2, vector<Velocity3D>* v2, Time stepsize);
};

#endif

