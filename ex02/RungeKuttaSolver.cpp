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

	float factors1[4] = {1.0f/6.0f, 1.0f/3.0f, 1.0f/3.0f, 1.0f/6.0f};
	float factors2[4] = {1.0f, 1.0f/2.0f, 1.0f/2.0f, 1.0f};

	Time t = _system->time;

	std::vector<Length3D> pos(_system->particles.size());
	std::vector<Velocity3D> vel(_system->particles.size());

	Length3D current_k_pos;
	Velocity3D current_k_vel;
	
	std::vector<Length3D> k_pos_accum(_system->particles.size());
	std::vector<Velocity3D> k_vel_accum(_system->particles.size());

	//backup position and velocity
	for(unsigned int par = 0; par < _system->particles.size(); par++)
	{
		k_vel_accum[par] = vel[par] = _system->particles[par].velocity;
		k_pos_accum[par] = pos[par] = _system->particles[par].position;
	}

	for(unsigned int midstep = 0; midstep < 4; midstep++)
	{
		_system->computeAccelerations();
		for(unsigned int par = 0; par < _system->particles.size(); par++)
		{
			current_k_vel = stepsize * _system->particles[par].acceleration;
			current_k_pos = stepsize * _system->particles[par].velocity;

			//save accumulated solutions
			k_vel_accum[par] += factors1[midstep] * current_k_vel;
			k_pos_accum[par] += factors1[midstep] * current_k_pos;

			if(midstep < 3)
			{
				_system->particles[par].velocity = vel[par] + factors2[midstep + 1] * current_k_vel;
				_system->particles[par].position = pos[par] + factors2[midstep + 1] * current_k_pos;
				_system->time = t + factors2[midstep + 1] * stepsize;
			}
		}
	}

	for(unsigned int par = 0; par < _system->particles.size(); par++)
	{
		_system->particles[par].velocity = k_vel_accum[par];
		_system->particles[par].position = k_pos_accum[par];
	}
	_system->time = t + stepsize;
}
