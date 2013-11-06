#ifndef _GRAVITATIONAL_SYSTEM_H
#define _GRAVITATIONAL_SYSTEM_H

#include "System.h"

/**
 * A system with gravitational interaction between the particles.
 */
class GravitationalSystem: public System {
	public:
		/**
		 * Construct a new system from a given list of particles.
		 *
		 * The particle list is only referenced and will be modified when the system is simulated.
		 */
		GravitationalSystem(std::vector<Particle> &particles);

		/**
		 * Compute the accelerations exerted on the particles.
		 *
		 * Accelerations are stored in the particles.
		 */
		void computeAccelerations();
};

#endif

