#include "MassSpringSystem.h"

MassSpringSystem::MassSpringSystem(std::vector<Particle> &particles, std::vector<Spring> &springs, ParticleDamping damping, Force wind_force, Time wind_period):
	System(particles), _springs(springs), _damping(damping), _wind_force(wind_force), _wind_period(wind_period) {}

void MassSpringSystem::computeAccelerations() {
    // set all accelerations to zero
    for(unsigned int i = 0; i < particles.size(); i++) {
        particles[i].acceleration[0] = 0*m/(s*s);
        particles[i].acceleration[1] = 0*m/(s*s);
        particles[i].acceleration[2] = 0*m/(s*s);
    }

    // apply the spring's forces to the connected particles
    for(unsigned int i = 0; i < _springs.size(); i++) {
        Force3D f = _springs[i].getForce();
        _springs[i].p1->acceleration += f/_springs[i].p1->mass;
        _springs[i].p2->acceleration -= f/_springs[i].p2->mass;
    }

    /*
	 * 2) Now take the damping of the particle motion, stored in "_damping", into account.
	 */

    // add gravity
    for(unsigned int i = 0; i < particles.size(); i++) {
        particles[i].acceleration[1] -= 9.81*m/(s*s);
    }

	/*
	 * 4) Set the accelerations of "fixed" particles to zero.
	 *
	 * 5) Finally, apply a crudely approximated wind acceleration. The magnitude of the acceleration is
	 * stored in "_wind_force", while the period of the oscillating wind direction is stored in
	 * "_wind_period".
	 */
}

