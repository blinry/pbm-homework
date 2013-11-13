#include "MassSpringSystem.h"

MassSpringSystem::MassSpringSystem(std::vector<Particle> &particles, std::vector<Spring> &springs, ParticleDamping damping, Force wind_force, Time wind_period):
	System(particles), _springs(springs), _damping(damping), _wind_force(wind_force), _wind_period(wind_period) {}

void MassSpringSystem::computeAccelerations() {
	/* 1) Please apply the forces of all springs in "_springs" to the particles attached to the spring.
	 * Remember that the getForce() function returns a force, not an acceleration, because the
	 * acceleration depends on the mass of the particle under consideration. Also, you only need to
	 * call getForce() once for each spring because the force for both attached particles only
	 * differs by the sign. Do not forget to set all accelerations to zero at the very beginning.
	 *
	 * 2) Now take the damping of the particle motion, stored in "_damping", into account.
	 *
	 * 3) Then add gravity.
	 *
	 * 4) Set the accelerations of "fixed" particles to zero.
	 *
	 * 5) Finally, apply a crudely approximated wind acceleration. The magnitude of the acceleration is
	 * stored in "_wind_force", while the period of the oscillating wind direction is stored in
	 * "_wind_period".
	 */
}

