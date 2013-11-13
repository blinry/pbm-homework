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

    for(unsigned int i = 0; i < particles.size(); i++) {
        // set the accelerations of "fixed" particles to zero
        if (particles[i].fixed) {
            particles[i].acceleration[0] = 0*m/(s*s);
            particles[i].acceleration[1] = 0*m/(s*s);
            particles[i].acceleration[2] = 0*m/(s*s);
            continue;
        }

        // damp particle
        particles[i].acceleration -= _damping*particles[i].velocity/particles[i].mass;

        // add gravity
        particles[i].acceleration[1] -= 9.81*m/(s*s);

        // wind!
        particles[i].acceleration[0] += (1.5 + cos(time/_wind_period))*_wind_force/particles[i].mass;
        particles[i].acceleration[2] += sin(time/_wind_period)*_wind_force/particles[i].mass;

    }
}

