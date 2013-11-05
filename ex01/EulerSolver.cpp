#include "EulerSolver.h"

void EulerSolver::step(const Time stepsize) {
	/* Please fill in your code for computing an explicit Euler time step here.
	 * The system on which the solver operates is available in the variable "_system".
	 * You have to let the system compute the accelerations exerted on all the
	 * particles before you can set the new position and velocity of each particle.
	 * Do not forget to increment the current time of the system when you are done.
	 */

	 _system->computeAccelerations();

	 for(unsigned int i = 0; i < _system->particles.size(); ++i)
	 {
		 //update velocity
		 _system->particles[i].velocity += stepsize * _system->particles[i].acceleration;

		 //update position
		 _system->particles[i].position += stepsize * _system->particles[i].velocity;
	 }

	 _system->time += stepsize;
}

