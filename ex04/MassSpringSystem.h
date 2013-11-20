#ifndef _MASS_SPRING_SYSTEM_H
#define _MASS_SPRING_SYSTEM_H

#include "System.h"
#include "Spring.h"
#include "Obstacle.h"

typedef Quantity<double, 1, 1, 0, -1> ParticleDamping;

/**
 * A system with gravitational interaction between the particles.
 */
class MassSpringSystem: public System {
	public:
		/**
		 * Construct a new system from a given list of particles.
		 *
		 * The particle list is only referenced and will be modified when the system is simulated.
		 */
		MassSpringSystem(std::vector<Particle> &particles, std::vector<Spring> &springs, std::vector<Obstacle*> &obstacles, ParticleDamping damping=ParticleDamping(0.0));

		/**
		 * Compute the accelerations exerted on the particles.
		 *
		 * Accelerations are stored in the particles.
		 */
		void computeAccelerations();

	private:
		std::vector<Spring> &_springs; //< The springs connecting the particles in the system.
		std::vector<Obstacle*> &_obstacles; //< The reflection obstacles for collision handling.
		ParticleDamping _damping; //< The per-particle damping coefficient.
};

#endif

