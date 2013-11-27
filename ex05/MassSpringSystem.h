#ifndef _MASS_SPRING_SYSTEM_H
#define _MASS_SPRING_SYSTEM_H

#include "System.h"
#include "Particle.h"
#include "Spring.h"

typedef Quantity<double, 1, 1, 0, -1> ParticleDamping;

/**
 * A system with gravitational interaction between the particles.
 */
class MassSpringSystem: public System<Particle<Length3D> > {
	public:
		/**
		 * Construct a new system from a given list of particles.
		 *
		 * The particle list is only referenced and will be modified when the system is simulated.
		 */
		MassSpringSystem(std::vector<particle_type> &particles, std::vector<Spring> &springs, ParticleDamping damping=ParticleDamping(0.0));

		/**
		 * Compute the accelerations exerted on the particles.
		 *
		 * Accelerations are stored in the particles.
		 */
		void computeAccelerations();

	private:
		std::vector<Spring> &_springs; //< The springs connecting the particles in the system.
		ParticleDamping _damping; //< The per-particle damping coefficient.
};

#endif

