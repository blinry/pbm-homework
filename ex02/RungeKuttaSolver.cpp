#include "RungeKuttaSolver.h"

void RungeKuttaSolver::step(const Time stepsize)  {
	/* Please fill in your code for computing a 4th order Runge-Kutta time step here.
	 * The system on which the solver operates is available in the variable "system".
	 *
	 * The system of differential equations that you will solve is of the form
	 *
	 *   y'(t) = f(y(t)),
	 *
	 * where
	 *
	 *   y(t) = (x(t), v(t)),
	 *
	 * with x(t) and v(t) containing the positions and velocities of all the
	 * particles, respectively. You can split this system into the position and
	 * velocity components, such that
	 *
	 *   x'(t) = v(t)  and
	 *   v'(t) = a(x(t)).
	 *
	 * You can evaluate a(x(t)) by setting all the particle positions in the system to
	 * the corresponding values of x(t), calling _system->computeAccelerations() and
	 * reading the accelerations back from the particles. If you want to be generic
	 * and allow for systems where the accelerations may also depend on the velocity
	 * (for example, when there is friction) or on the time (for example, when an
	 * external force changes over time), you should also set the particle velocities
	 * and the system time before calling computeAccelerations().
	 *
	 * When your computation is done, store the new x and v values in the particles.
	 * Do not forget to increment the current time of the system when you are done.
	 */
}

