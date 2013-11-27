#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <vector>
#include "Quantity.h"

using namespace quantity;

/**
 * Abstract base class for particle systems and their equations of motion.
 */
template <typename T>
class System {
	public:
		typedef T particle_type;

		/**
		 * Construct a new system from a given list of particles.
		 *
		 * The particle list is only referenced and will be modified when the system is simulated.
		 */
		System(std::vector<particle_type> &particles): particles(particles) {}

		virtual ~System() {}

		/**
		 * Compute the accelerations exerted on the particles.
		 *
		 * Accelerations are stored in the particles.
		 */
		virtual void computeAccelerations() = 0;

		std::vector<particle_type> &particles; //< A reference to the list of particles.
		Time time; //< The current time step.
};

#endif

