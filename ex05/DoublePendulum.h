#ifndef _DOUBLE_PENDULUM_H
#define _DOUBLE_PENDULUM_H

#include "System.h"
#include "Particle.h"

/**
 * A double pendulum.
 */
class DoublePendulum: public System<Particle<Angle> > {
	public:
		/**
		 * Construct a new system from a given list of particles.
		 *
		 * The particle list is only referenced and will be modified when the system is simulated.
		 */
		DoublePendulum(std::vector<particle_type> &particles, const Length &l1, const Length &l2);

		/**
		 * Compute the accelerations exerted on the particles.
		 *
		 * Accelerations are stored in the particles.
		 */
		void computeAccelerations();

		Length l1, l2;
};

#endif

