#include "AdaptiveEulerSolver.h"
using namespace std;

void AdaptiveEulerSolver::step(const Time stepsize) {
	/* Please fill in your code for computing an adaptive explicit Euler time step here.
	 * The system on which the solver operates is available in the variable "_system".
	 * The average error threshold is available in the variable "_threshold".
	 * You have to let the system compute the accelerations exerted on all the
	 * particles before you can set the new position and velocity of each particle.
	 * Do not forget to increment the current time of the system when you are done.
	 */

	Time passed_time = 0 * s;

	//backup position and velocity
	std::vector<Length3D> pos(_system->particles.size());
	std::vector<Velocity3D> vel(_system->particles.size());

	//do steps until enough time has passed
	while(passed_time < stepsize)
	{
		std::vector<Length3D> r1, r2_2;
		for(unsigned int i = 0; i < _system->particles.size(); i++)
		{
			pos[i] = _system->particles[i].position;
			vel[i] = _system->particles[i].velocity;
		}

		//do explicit euler step with full stepsize
		for(unsigned int i = 0; i < _system->particles.size(); i++)
		{
			r1.push_back(pos[i] + stepsize * vel[i]);
		}

		//do two explicit euler steps with half of stepsize
		_system->computeAccelerations();
		for(unsigned int i = 0; i < _system->particles.size(); i++)
		{
			Velocity3D nextVel = vel[i] + 0.5 * stepsize * _system->particles[i].acceleration;
			Length3D nextPos = pos[i] + 0.5 * stepsize * _system->particles[i].velocity;
			r2_2.push_back(nextPos + 0.5 * stepsize * nextVel);
		}

		//maximum distance between r1 and r2_2
		Length max = 0 * m;
		for(unsigned int i = 0; i < _system->particles.size(); i++)
		{
			Length dist = norm(r1[i] - r2_2[i]);
			if(dist > max)
				max = dist;
		}

		//hmax from lecture 2 slide 22
		Time hmax = 0 * s;
		if(max > 0 * m)
			hmax = stepsize * sqrt(_threshold / max);
		else
			hmax = stepsize; 

		//do euler step with hmax stepsize
		//_system->computeAccelerations() //was already called, nothing changed
		for(unsigned int i = 0; i < _system->particles.size(); i++)
		{
			_system->particles[i].position = pos[i] + hmax * vel[i];
			_system->particles[i].velocity = vel[i] + hmax * _system->particles[i].acceleration;
		}

		//set next clock time
		_system->time += hmax;
		passed_time += hmax;
	}
}

        // do two Euler steps of half stepsize, store resulting positions in r2_2
        _system->computeAccelerations();
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r2_1.push_back(p->position + .5*stepsize*p->velocity);
            v2.push_back(p->velocity + .5*stepsize*p->acceleration);
            p->position = r2_1[i];
        }
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r2_2.push_back(r2_1[i] + .5*stepsize*v2[i]);
        }

        // find maximal distance between the two calculations
        Length max;
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Length d = norm(r1[i]-r2_2[i]);
            if (d > max) {
                max = d;
            }
        }

        // calculate step size 'hmax' (from the lecture, slide 22)
        Time hmax;
        Length null;
        if (max > null ) {
            hmax = stepsize*sqrt(_threshold/max);
        } else {
            hmax = stepsize;
        }

        // do an Euler step of this step size
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            p->position = r0[i];
        }
        _system->computeAccelerations();
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            p->position += hmax*v0[i];
            p->velocity += hmax*p->acceleration;
        }

        _system->time += hmax;
        passed_time += hmax;
    }
}
