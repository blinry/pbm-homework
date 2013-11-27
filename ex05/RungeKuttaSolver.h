#ifndef _RUNGE_KUTTA_SOLVER_H
#define _RUNGE_KUTTA_SOLVER_H

#include "Solver.h"

/**
 * Runge-Kutta method for solving the equations of motion of a system.
 */
template <typename T>
class RungeKuttaSolver: public Solver<T> {
	public:
		/**
		 * Construct a new solver for a given system.
		 */
		RungeKuttaSolver(System<T> *system): Solver<T>(system) {}

		/**
		 * Iterate the system by a fixed timestep.
		 */
		void step(const Time stepsize) {
			// Store old values of x and v.
			std::vector<typename T::length_type> x0;
			std::vector<typename T::velocity_type> v0;
			for (size_t i = 0; i < Solver<T>::_system->particles.size(); ++i) {
				x0.push_back(Solver<T>::_system->particles[i].position);
				v0.push_back(Solver<T>::_system->particles[i].velocity);
			}

			// Compute a(x0).
			Solver<T>::_system->computeAccelerations();
			std::vector<typename T::velocity_type> dx1;
			std::vector<typename T::acceleration_type> dv1;
			for (size_t i = 0; i < Solver<T>::_system->particles.size(); ++i) {
				// Compute  dx1 = v0  and  dv1 = a(x0).
				dx1.push_back(Solver<T>::_system->particles[i].velocity);
				dv1.push_back(Solver<T>::_system->particles[i].acceleration);
				// Set position and velocity for computation of dx2 and dv2.
				Solver<T>::_system->particles[i].position = x0[i] + 0.5 * stepsize * dx1[i];
				Solver<T>::_system->particles[i].velocity = v0[i] + 0.5 * stepsize * dv1[i];
			}

			// Set  t = t0 + 1/2 h.
			Solver<T>::_system->time += 0.5 * stepsize;

			// Compute a(x0 + 1/2 h dx1).
			Solver<T>::_system->computeAccelerations();
			std::vector<typename T::velocity_type> dx2;
			std::vector<typename T::acceleration_type> dv2;
			for (size_t i = 0; i < Solver<T>::_system->particles.size(); ++i) {
				// Compute  dx2 = v0 + 1/2 h dv1  and  dv2 = a(x0 + 1/2 h dx1).
				dx2.push_back(Solver<T>::_system->particles[i].velocity);
				dv2.push_back(Solver<T>::_system->particles[i].acceleration);
				// Set position and velocity for computation of dx3 and dv3.
				Solver<T>::_system->particles[i].position = x0[i] + 0.5 * stepsize * dx2[i];
				Solver<T>::_system->particles[i].velocity = v0[i] + 0.5 * stepsize * dv2[i];
			}

			// Compute a(x0 + 1/2 h dx2).
			Solver<T>::_system->computeAccelerations();
			std::vector<typename T::velocity_type> dx3;
			std::vector<typename T::acceleration_type> dv3;
			for (size_t i = 0; i < Solver<T>::_system->particles.size(); ++i) {
				// Compute  dx3 = v0 + 1/2 h dv2  and  dv3 = a(x0 + 1/2 h dx2).
				dx3.push_back(Solver<T>::_system->particles[i].velocity);
				dv3.push_back(Solver<T>::_system->particles[i].acceleration);
				// Set position and velocity for computation of dx4 and dv4.
				Solver<T>::_system->particles[i].position = x0[i] + stepsize * dx3[i];
				Solver<T>::_system->particles[i].velocity = v0[i] + stepsize * dv3[i];
			}

			// Set  t = t0 + h.
			Solver<T>::_system->time += 0.5 * stepsize;

			// Compute a(x0 + h dx3).
			Solver<T>::_system->computeAccelerations();
			std::vector<typename T::velocity_type> dx4;
			std::vector<typename T::acceleration_type> dv4;
			for (size_t i = 0; i < Solver<T>::_system->particles.size(); ++i) {
				// Compute  dx4 = v0 + h dv3  and  dv4 = a(x0 + h dx3).
				dx4.push_back(Solver<T>::_system->particles[i].velocity);
				dv4.push_back(Solver<T>::_system->particles[i].acceleration);
				// Set  x = x0 + 1/6 h (dx1 + 2 dx2 + 2 dx3 + dx4)  and  v = v0 + 1/6 h (dv1 + 2 dv2 + 2 dv3 + dv4).
				Solver<T>::_system->particles[i].position = x0[i] + stepsize / 6.0 * (dx1[i] + 2.0 * dx2[i] + 2.0 * dx3[i] + dx4[i]);
				Solver<T>::_system->particles[i].velocity = v0[i] + stepsize / 6.0 * (dv1[i] + 2.0 * dv2[i] + 2.0 * dv3[i] + dv4[i]);
			}
		}
};

#endif

